/**
 * SNES to DB9 Joystick converter
 *
 * (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
 * http://www.asmsoftware.de/
 *
 * The MIT License applies to this software. See COPYING for details.
 *
 * @file    version.h
 * @brief   Software version information definition
 * @details This header is supposed to be "touched" by the build process to ensure up-to-date timestamps.
 *
 */
 
#ifndef VERSION_H
	#define VERSION_H
	
	#define SWVERSION "V0.9.3"
	
	#define COPYRIGHTSTR "SNES2DB9 ATtiny84 " SWVERSION " - (c) 2020 by Matthias Arndt <marndt@asmsoftware.de> Build: " __TIME__ "-" __DATE__
	
#endif
