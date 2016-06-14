curly braces - start in same line
spaces between function name and paramter list
return values of function in separate line
empty function parameter list explicitly state with (void)
class member variable prefixed with m_
class member function invocaktion prefixed with this->
	call static class function without this->
when checking for boolean expression write the constant value first
	bad: if (count == 0) {...} (could lead to if (count = 0) {...})
	good: if (0 == count) {...} or while (0 < countdown) {...}
when checking for a false expression avoid the ! operator
	bad: if (!valid) {...}
	good: if (false == valid) {...}
attach pointer or reference type indicater to base type
	bad: std::string &str
	good: std::string& str
use const in member functions not chaning any internal state
	bad: 
		bool
		is_valid (void) {
			return glIsShader (m_handle);
		}
	good:
		bool
		is_valid (void) const {
			return glIsShader (m_handle);
		}
use namespaced or at least classname based guard symbols
	dont use uppercase only symbols
	dont use underscored symbols
	bad:
		#ifndef __MYCLASS__
		#define __MYCLASS__
		namespace myspace {
		class MyClass {};
		}
		#endif
	good:
		#ifndef myspace_MyClass_h
		#define myspace_MyClass_h
		namespace myspace {
		class MyClass {};
		}
		#endif