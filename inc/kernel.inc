


EVENT_TIMEOUT   equ 0
EVENT_MESSAGE   equ 1
EVENT_OBJECT    equ 2
EVENT_QUEUE     equ 4
EVENT_SIGNAL    equ 5

EVENT_MESSAGE_BITPOS    equ 0
EVENT_OBJECT_BITPOS     equ 1
EVENT_QUEUE_BITPOS      equ 2

TASK_STATE_OFFSET equ 3
;Region must be right after the resource (some code in SysCallFunction depends on this)
TASK_REGION_LIST_OFFSET equ 8