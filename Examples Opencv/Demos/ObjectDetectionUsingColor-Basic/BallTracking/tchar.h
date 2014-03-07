#ifndef __TCHAR__
#define __TCHAR__


	#include <stdio.h>
	#include <stdarg.h>
	#include <string>
	#include <string.h>

	//Unicode for unix
	#if ((defined(UNICODE) || defined(_UNICODE)) && defined(__unix__))
		#include <wchar.h>
		#define LPCTSTR const wchar_t*
		#define LPTSTR wchar_t*
		#define TCHAR wchar_t

		//Functions
		#define _T(x) Lx
		#define _tcslen	wcslen
		#define _stprintf_s swprintf
		#define _vstprintf vswprintf

		#define _tcscpy wcscpy
		#define _tcsncpy wcsncpy

		#define _tcstok_s wcstok

	//ASCII for unix
	#elif __unix__ 
		#define LPCTSTR const char*
		#define LPTSTR char*
		#define TCHAR char

		//Functions
		#define _T(x) x
		#define _tcslen	strlen
		#define _stprintf_s snprintf
		#define _tprintf
		#define _vstprintf vsprintf

		#define _tcscpy strcpy
		#define _tcsncpy strncpy

		#define _tcstok_s strtok_r

	#endif
#endif