;screwball method of getting to the chicken without the egg.

;Initially, this is linked in so that we can build all the way and end up with an abs file.
;The code constructs are extracted and a resource.bin/asm is built.  That file is then assembled and linked
;in instead of this one.  Hopefully it all works. :)

    include "project.inc"
        global FResource
        org y,".yconstresource":
FResource
    ds  RESOURCE_FILE_SIZE
