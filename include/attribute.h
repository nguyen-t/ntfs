#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <sys/types.h>
#include "mft.h"
#include "attr_type.h"
#include "attr_def.h"

Attribute* attribute_next(MFT*);
void attribute_print(Attribute*);

#endif
