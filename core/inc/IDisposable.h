#pragma once

#ifndef IDisposable_h
#define IDisposable_h

struct IDisposable {
	virtual
	~IDisposable (void) {}

	virtual void
	on_dispose (void) = 0;
};

#endif
