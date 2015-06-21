#ifndef __IRESOURCE_H__
#define __IRESOURCE_H__

class IResource {

// TODO: think about a resouce baseclass used to track
// allocated resouces and their disposal (and any lack thereof)

public:
	virtual
	~IResource (void) {}

};

#endif
