CFLAGS += -std=c99 -Wall -Wextra -pedantic -Wold-style-declaration
CFLAGS += -Wno-unused-parameter
PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin
MANDIR  = $(PREFIX)/share/man
CC     ?= gcc

all:
	@echo RUN \'make install\' to install crem
	@echo RUN \'make uninstall\' to uninstall crem
	@echo RUN \'make manpage\' to generate a man page for crem

manpage:
	@if [ -f "/usr/bin/pandoc" ]; then \
		pandoc -s -t man man/crem.1.md -o man/crem.1; \
		echo "SUCCESS: manpage generated"; \
	else \
		echo "ERROR: could not generate manpage. Pandoc not found."; \
	fi

crem:
	@$(CC) $(CFLAGS) -o $@ src/crem.c

install: crem
	@install -Dm755 crem $(DESTDIR)$(BINDIR)/crem
	@rm -f crem
	@mkdir -p $(DESTDIR)$(MANDIR)/man1
	@cp -p man/crem.1 $(DESTDIR)$(MANDIR)/man1

uninstall:
	@rm -f $(DESTDIR)$(BINDIR)/crem
	@rm -rf $(DESTDIR)$(MANDIR)/man1/crem.1*
