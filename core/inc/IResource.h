#ifndef IResource_H
#define IResource_H

class IResource {

// TODO: think about a resouce baseclass used to track
// allocated resouces and their disposal (and any lack thereof)

public:
	virtual void
	dispose (void) = 0;

	virtual
	~IResource (void) {}

};

#endif
