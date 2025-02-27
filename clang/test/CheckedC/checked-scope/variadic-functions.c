// Test calls to variadic functions in checked scopes.
// Some -Wformat error messages are different between linux and windows
// systems. This file contains tests that have the same error messages on both
// linux and windows. The windows-specific tests are in
// variadic-functions-win.c and the non-windows tests are in
// variadic-functions-non-win.c.

// RUN: %clang_cc1 -fcheckedc-extension -verify \
// RUN: -verify-ignore-unexpected=note %s

typedef __builtin_va_list va_list;
typedef __WCHAR_TYPE__ wchar_t;
typedef __SIZE_TYPE__ size_t;
typedef _Ptr<char> FILE;

int printf(const char *format : itype(_Nt_array_ptr<const char>), ...);
int MyPrintf(const char *format : itype(_Nt_array_ptr<const char>), ...)
  __attribute__((format(printf, 1, 2)));

int scanf(const char *format : itype(_Nt_array_ptr<const char>), ...);
int MyScanf(const char *format : itype(_Nt_array_ptr<const char>), ...)
  __attribute__((format(scanf, 1, 2)));

int fprintf(FILE *stream : itype(_Ptr<FILE>),
            const char *format : itype(_Nt_array_ptr<const char>), ...);
int sprintf(char *s,
            const char *format : itype(_Nt_array_ptr<const char>), ...);

int snprintf(char *s : itype(_Nt_array_ptr<char>) count(n-1),
             size_t n _Where n > 0,
             const char *format : itype(_Nt_array_ptr<const char>), ...);

int fscanf(FILE *stream : itype(_Ptr<FILE>),
           const char *format : itype(_Nt_array_ptr<const char>), ...);
int sscanf(const char *s : itype(_Nt_array_ptr<const char>),
           const char *format : itype(_Nt_array_ptr<const char>), ...);

int vprintf(const char *format : itype(_Nt_array_ptr<const char>),
            va_list arg);
int vfprintf(FILE *stream : itype(_Ptr<FILE>),
             const char *format : itype(_Nt_array_ptr<const char>),
             va_list arg);
int vsprintf(char *s,
             const char *format : itype(_Nt_array_ptr<const char>),
             va_list arg);

int vscanf(const char *format : itype(_Nt_array_ptr<const char>),
            va_list arg);
int vfscanf(FILE *stream : itype(_Ptr<FILE>),
            const char *format : itype(_Nt_array_ptr<const char>),
            va_list arg);
int vsscanf(const char *s : itype(_Nt_array_ptr<const char>),
            const char *format : itype(_Nt_array_ptr<const char>),
            va_list arg);

void f1(_Nt_array_ptr<char> chk, char *unchk1, _Array_ptr<char> arr) {
  int a = 1, b = 2;
  char *unchk2;

  printf("%d %s %d %s", 1, unchk1, 2, unchk2);
  MyPrintf("%d %s %d %s", 1, unchk1, 2, unchk2);
  scanf("%d %s %d %s", &a, unchk1, &b, unchk2);
  MyScanf("%d %s %d %s", &a, unchk1, &b, unchk2);

_Checked {
  printf("%d %s %d %s", 1, unchk1, 2, unchk2); // expected-error {{local variable used in a checked scope must have a checked type}} expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  MyPrintf("%d %s %d %s", 1, unchk1, 2, unchk2); // expected-error {{local variable used in a checked scope must have a checked type}} expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  scanf("%d %s %d %s", &a, unchk1, &b, unchk2); // expected-error {{local variable used in a checked scope must have a checked type}} expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  MyScanf("%d %s %d %s", &a, unchk1, &b, unchk2); // expected-error {{local variable used in a checked scope must have a checked type}} expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}

  printf("%s", arr); // expected-error {{in a checked scope %s format specifier requires null-terminated argument}}
  MyPrintf("%s", arr); // expected-error {{in a checked scope %s format specifier requires null-terminated argument}}
  scanf("%s", arr); // expected-error {{in a checked scope %s format specifier requires null-terminated argument}}
  MyScanf("%s", arr); // expected-error {{in a checked scope %s format specifier requires null-terminated argument}}
}
}

void f2 (_Nt_array_ptr<char> p, _Array_ptr<char> arr, _Ptr<FILE> fp) {
  char *s;

_Checked {
  fprintf(fp, "%s", arr); // expected-error {{in a checked scope %s format specifier requires null-terminated argument}}
  sprintf(s, "%s", arr); // expected-error {{local variable used in a checked scope must have a checked type}}
  snprintf(p, 1, "%s", arr); // expected-error {{in a checked scope %s format specifier requires null-terminated argument}}

  fscanf(fp, "%s", arr); // expected-error {{in a checked scope %s format specifier requires null-terminated argument}}
  sscanf(p, "%s", arr); // expected-error {{in a checked scope %s format specifier requires null-terminated argument}}
}
}

void f3(_Nt_array_ptr<char> p, _Array_ptr<char> arr, _Ptr<FILE> fp) {
  va_list args;
  char *s;

_Checked {
  vprintf(p, args); // expected-error {{local variable used in a checked scope must have a checked type}}
  vfprintf(fp, "%s", args); // expected-error {{local variable used in a checked scope must have a checked type}}
  vsprintf(s, "%s", args); // expected-error {{local variable used in a checked scope must have a checked type}} expected-error {{local variable used in a checked scope must have a checked type}}

  vscanf(p, args); // expected-error {{local variable used in a checked scope must have a checked type}}
  vfscanf(fp, "%s", args); // expected-error {{local variable used in a checked scope must have a checked type}}
  vsscanf(s, "%s", args); // expected-error {{local variable used in a checked scope must have a checked type}} expected-error {{local variable used in a checked scope must have a checked type}}
}
}

void f4 (_Nt_array_ptr<char> p, _Nt_array_ptr<wchar_t> w, _Ptr<int> ptr, _Ptr<void> voidPtr) {
_Checked {
  printf(p); // expected-error {{format string is not a string literal (potentially insecure)}}
  MyPrintf(p); // expected-error {{format string is not a string literal (potentially insecure)}}
  scanf(p); // expected-error {{format string is not a string literal (potentially insecure)}}
  MyScanf(p); // expected-error {{format string is not a string literal (potentially insecure)}}

  printf(p, ""); // expected-error {{format string is not a string literal}}
  MyPrintf(p, ""); // expected-error {{format string is not a string literal}}
  scanf(p, ""); // expected-error {{format string is not a string literal}}
  MyScanf(p, ""); // expected-error {{format string is not a string literal}}

  printf("%d"); // expected-error {{more '%' conversions than data arguments}}
  MyPrintf("%d"); // expected-error {{more '%' conversions than data arguments}}
  scanf("%d"); // expected-error {{more '%' conversions than data arguments}}
  MyScanf("%d"); // expected-error {{more '%' conversions than data arguments}}

  printf("%d", 1, 2); // expected-error {{data argument not used by format string}}
  MyPrintf("%d", 1, 2); // expected-error {{data argument not used by format string}}
  scanf("%d", 1, 2); // expected-error {{format specifies type 'int *' but the argument has type 'int'}} expected-error {{data argument not used by format string}}
  MyScanf("%d", 1, 2); // expected-error {{format specifies type 'int *' but the argument has type 'int'}} expected-error {{data argument not used by format string}}

  printf("\%", p); // expected-error {{incomplete format specifier}}
  MyPrintf("\%", p); // expected-error {{incomplete format specifier}}
  scanf("\%", p); // expected-error {{incomplete format specifier}}
  MyScanf("\%", p); // expected-error {{incomplete format specifier}}

  printf("", p); // expected-error {{format string is empty}}
  MyPrintf("", p); // expected-error {{format string is empty}}
  scanf("", p); // expected-error {{format string is empty}}
  MyScanf("", p); // expected-error {{format string is empty}}

  printf("%10s", p);
  MyPrintf("%10s", p);
  scanf("%10s", p); // expected-error {{in a checked scope width is not allowed with format specifier in scanf}}
  MyScanf("%10s", p); // expected-error {{in a checked scope width is not allowed with format specifier in scanf}}

  printf("%d", 1.0); // expected-error {{format specifies type 'int' but the argument has type 'double'}}
  MyPrintf("%d", 1.0); // expected-error {{format specifies type 'int' but the argument has type 'double'}}
  scanf("%d", 1.0); // expected-error {{format specifies type 'int *' but the argument has type 'double'}}
  MyScanf("%d", 1.0); // expected-error {{format specifies type 'int *' but the argument has type 'double'}}

  printf("%1$d", 1); // expected-error {{positional arguments are not supported by ISO C}}
  MyPrintf("%1$d", 1); // expected-error {{positional arguments are not supported by ISO C}}
  scanf("%1$d", 1); // expected-error {{positional arguments are not supported by ISO C}} expected-error {{format specifies type 'int *' but the argument has type 'int'}}
  MyScanf("%1$d", 1); // expected-error {{positional arguments are not supported by ISO C}} expected-error {{format specifies type 'int *' but the argument has type 'int'}}

  printf("%2$d", 1); // expected-error {{positional arguments are not supported by ISO C}} expected-error {{data argument position '2' exceeds the number of data arguments (1)}}
  MyPrintf("%2$d", 1); // expected-error {{positional arguments are not supported by ISO C}} expected-error {{data argument position '2' exceeds the number of data arguments (1)}}
  scanf("%2$d", 1); // expected-error {{positional arguments are not supported by ISO C}} expected-error {{data argument position '2' exceeds the number of data arguments (1)}}
  MyScanf("%2$d", 1); // expected-error {{positional arguments are not supported by ISO C}} expected-error {{data argument position '2' exceeds the number of data arguments (1)}}

  printf("%1$d %d", 4, 4); // expected-error {{positional arguments are not supported by ISO C}} expected-error {{cannot mix positional and non-positional arguments in format string}}
  MyPrintf("%1$d %d", 4, 4); // expected-error {{positional arguments are not supported by ISO C}} expected-error {{cannot mix positional and non-positional arguments in format string}}
  scanf("%1$d %d", 4, 4); // expected-error {{positional arguments are not supported by ISO C}} expected-error {{cannot mix positional and non-positional arguments in format string}} expected-error {{format specifies type 'int *' but the argument has type 'int'}}
  MyScanf("%1$d %d", 4, 4); // expected-error {{positional arguments are not supported by ISO C}} expected-error {{cannot mix positional and non-positional arguments in format string}} expected-error {{format specifies type 'int *' but the argument has type 'int'}}

  printf("\0%d", 1); // expected-error {{format string contains '\0' within the string body}}
  MyPrintf("\0%d", 1); // expected-error {{format string contains '\0' within the string body}}
  scanf("\0%d", 1); // expected-error {{format string contains '\0' within the string body}}
  MyScanf("\0%d", 1); // expected-error {{format string contains '\0' within the string body}}

  printf("%Lc", 'a'); // expected-error {{length modifier 'L' results in undefined behavior or no effect with 'c' conversion specifier}}
  MyPrintf("%Lc", 'a'); // expected-error {{length modifier 'L' results in undefined behavior or no effect with 'c' conversion specifier}}
  scanf("%Lc", 'a'); // expected-error {{length modifier 'L' results in undefined behavior or no effect with 'c' conversion specifier}}
  MyScanf("%Lc", 'a'); // expected-error {{length modifier 'L' results in undefined behavior or no effect with 'c' conversion specifier}}

  printf("%P", p); // expected-error {{invalid conversion specifier 'P'}}
  MyPrintf("%P", p); // expected-error {{invalid conversion specifier 'P'}}
  scanf("%P", p); // expected-error {{invalid conversion specifier 'P'}}
  MyScanf("%P", p); // expected-error {{invalid conversion specifier 'P'}}

  printf("%S", w); // expected-error {{'S' conversion specifier is not supported by ISO C}}
  MyPrintf("%S", w); // expected-error {{'S' conversion specifier is not supported by ISO C}}
  scanf("%S", w); // expected-error {{'S' conversion specifier is not supported by ISO C}}
  MyScanf("%S", w); // expected-error {{'S' conversion specifier is not supported by ISO C}}

  printf("%*d"); // expected-error {{'*' specified field width is missing a matching 'int' argument}}
  MyPrintf("%*d"); // expected-error {{'*' specified field width is missing a matching 'int' argument}}
  scanf("%*d"); // Note: This is safe because * indicates the data is to be read from the stream but ignored (i.e. it is not stored in the location pointed by an argument).
  MyScanf("%*d"); // Note: This is safe because * indicates the data is to be read from the stream but ignored (i.e. it is not stored in the location pointed by an argument).

  printf("%.*d"); // expected-error {{'.*' specified field precision is missing a matching 'int' argument}}
  MyPrintf("%.*d"); // expected-error {{'.*' specified field precision is missing a matching 'int' argument}}
  scanf("%.*d"); // expected-error {{invalid conversion specifier '.'}}
  MyScanf("%.*d"); // expected-error {{invalid conversion specifier '.'}}

  printf("%.3p", voidPtr); // expected-error {{precision used with 'p' conversion specifier, resulting in undefined behavior}}
  scanf("%.3p", voidPtr); // expected-error {{invalid conversion specifier '.'}}
  MyScanf("%.3p", voidPtr); // expected-error {{invalid conversion specifier '.'}}

  printf("%+p", voidPtr); // expected-error {{flag '+' results in undefined behavior with 'p' conversion specifier}}
  MyPrintf("%+p", voidPtr); // expected-error {{flag '+' results in undefined behavior with 'p' conversion specifier}}
  scanf("%+p", voidPtr); // expected-error {{invalid conversion specifier '+'}}
  MyScanf("%+p", voidPtr); // expected-error {{invalid conversion specifier '+'}}

  printf("%{private}s", p); // expected-error {{using 'private' format specifier annotation outside of os_log()/os_trace()}}
  MyPrintf("%{private}s", p); // expected-error {{using 'private' format specifier annotation outside of os_log()/os_trace()}}
  scanf("%{private}s", p); // expected-error {{invalid conversion specifier '{'}}
  MyScanf("%{private}s", p); // expected-error {{invalid conversion specifier '{'}}

  printf("% +f", 1.23); // expected-error {{flag ' ' is ignored when flag '+' is present}}
  MyPrintf("% +f", 1.23); // expected-error {{flag ' ' is ignored when flag '+' is present}}
  scanf("% +f", 1.23); // expected-error {{invalid conversion specifier ' '}}
  MyScanf("% +f", 1.23); // expected-error {{invalid conversion specifier ' '}}

  printf("%c", (_Bool)1); // expected-error {{using '%c' format specifier, but argument has boolean value}}
  MyPrintf("%c", (_Bool)1); // expected-error {{using '%c' format specifier, but argument has boolean value}}
  scanf("%c", (_Bool)1); // expected-error {{format specifies type 'char *' but the argument has type 'int'}}
  MyScanf("%c", (_Bool)1); // expected-error {{format specifies type 'char *' but the argument has type 'int'}}

  printf("%]", p); // expected-error {{invalid conversion specifier ']'}}
  MyPrintf("%]", p); // expected-error {{invalid conversion specifier ']'}}
  scanf("%]", p); // expected-error {{invalid conversion specifier ']'}}
  MyScanf("%]", p); // expected-error {{invalid conversion specifier ']'}}
}
}

int MyPrintf_NoFormatAttr(const char *format : itype(_Nt_array_ptr<const char>), ...);

void f5(_Nt_array_ptr<char> p) {
_Checked {
  MyPrintf_NoFormatAttr("%s", "abc"); // expected-error {{cannot use this variable arguments function in a checked scope or function}}
}
}
