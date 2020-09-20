# SNES to DB9 Joystick converter
#
# (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
# http://www.asmsoftware.de/
#
# The MIT License applies to this software. See COPYING for details.
#
# Makefile to generate graphs from .dot fiels with dot and graphviz tool
#

all: system_diagram.png SNESReader_fsm.png

%.png: %.dot
	dot -Tpng -Kdot $< -o $@
	
	
