//
// Copyright (c) 2021-2022, Manticore Software LTD (http://manticoresearch.com)
// Copyright (c) 2001-2016, Andrew Aksyonoff
// Copyright (c) 2008-2016, Sphinx Technologies Inc
// All rights reserved
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License. You should have
// received a copy of the GPL license along with this program; if you
// did not, you can find it at http://www.gnu.org/
//

#include "conversion.h"

#include <cmath>
#include <cfloat>


DWORD sphToDword ( const char * s )
{
	if ( !s ) return 0;
	return strtoul ( s, NULL, 10 );
}


// move to source mysql
float sphToFloat ( const char * s )
{
	if ( !s ) return 0.0f;
	double fRes = strtod ( s, NULL );
	if ( fRes==-HUGE_VAL || fRes<=-FLT_MAX )
		return -FLT_MAX;

	if ( fRes==HUGE_VAL || fRes>=FLT_MAX )
		return FLT_MAX;

	return (float)fRes;
}


int64_t sphToInt64 ( const char * szNumber, CSphString * pError )
{
	if ( !szNumber )
		return 0;

	char * szEndPtr = nullptr;
	errno = 0;

	int64_t iNumber = strtoll ( szNumber, &szEndPtr, 10 );
	if ( pError )
	{
		if ( szNumber==szEndPtr )
			pError->SetSprintf ( "invalid number \"%s\", " INT64_FMT " assumed", szNumber, iNumber );
		else if ( errno==ERANGE && iNumber==LONG_MIN )
			pError->SetSprintf ( "underflow detected \"%s\", " INT64_FMT " assumed", szNumber, iNumber );
		else if ( errno==ERANGE && iNumber==LONG_MAX )
			pError->SetSprintf ( "overflow detected \"%s\", " INT64_FMT " assumed", szNumber, iNumber );
	}

	return iNumber;
}
