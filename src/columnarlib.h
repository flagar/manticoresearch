//
// Copyright (c) 2020-2022, Manticore Software LTD (http://manticoresearch.com)
// All rights reserved
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License. You should have
// received a copy of the GPL license along with this program; if you
// did not, you can find it at http://www.gnu.org/
//

#ifndef _columnarlib_
#define _columnarlib_

#include "sphinxexpr.h"

#include "columnar/columnar.h"
#include "columnar/builder.h"

class ISphSchema;

std::unique_ptr<columnar::Columnar_i>	CreateColumnarStorageReader ( const CSphString & sFile, DWORD uNumDocs, CSphString & sError );
std::unique_ptr<columnar::Builder_i>	CreateColumnarBuilder ( const ISphSchema & tSchema, const columnar::Settings_t & tSettings, const CSphString & sFilename, CSphString & sError );
void					CheckColumnarStorage ( const CSphString & sFile, DWORD uNumRows, std::function<void (const char*)> fnError, std::function<void (const char*)> fnProgress );

columnar::AttrType_e	ToColumnarType ( ESphAttr eAttrType, int iBitCount );

bool			InitColumnar ( CSphString & sError );
void			ShutdownColumnar();
const char *	GetColumnarVersionStr();
int				GetColumnarStorageVersion();
bool			IsColumnarLibLoaded();


// params rearranged for most frequently used defaults
inline std::unique_ptr<columnar::Iterator_i> CreateIterator ( const columnar::Columnar_i* pColumnar, const std::string& sName, std::string& sError, const columnar::IteratorHints_t& tHints={}, columnar::IteratorCapabilities_t* pCapabilities=nullptr )
{
	return std::unique_ptr<columnar::Iterator_i> { pColumnar->CreateIterator ( sName, tHints, pCapabilities, sError ) };
}


#endif // _columnarlib_
