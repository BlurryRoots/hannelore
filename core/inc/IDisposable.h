#pragma once

#ifndef IDisposable_h
#define IDisposable_h

struct IDisposable {
	virtual
	~IDisposable (void) {}

	virtual void
	dispose (void) = 0;
};

#endif
