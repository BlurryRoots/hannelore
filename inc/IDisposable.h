#ifndef IDISPOSABLE_H
#define IDISPOSABLE_H

struct IDisposable {
	virtual
	~IDisposable (void) {}

	virtual void
	dispose (void) = 0;
};

#endif
