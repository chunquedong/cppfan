//
//  TextCodec.hpp
//  cppfan
//
//  Created by 杨建东 on 2016/12/22.
//  Copyright © 2016年 yangjiandong. All rights reserved.
//

#ifndef TextCodec_hpp
#define TextCodec_hpp

#include "cppfan/base/common.h"

CF_BEGIN_NAMESPACE

namespace TextCodec {

size_t utf8ToUnicode(const char *s, wchar_t *out, size_t limit );
size_t unicodeToUtf8(const wchar_t *w, char *out, size_t limit );

size_t ansiToUnicode(const char *s, wchar_t *out, size_t limit );
size_t unicodeToAnsi(const wchar_t *w, char *out, size_t limit );

size_t ansiToUtf8(const char *in, char *out, size_t limit );
size_t utf8ToAnsi(const char *in, char *out, size_t limit );

}

CF_END_NAMESPACE

#endif /* TextCodec_hpp */
