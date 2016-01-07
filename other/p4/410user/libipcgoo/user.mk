410U_IPCGOO_OBJS := exec.o  print.o  readline.o 
410U_IPCGOO_OBJS := $(410U_IPCGOO_OBJS:%=$(410UDIR)/libipcgoo/%)
ALL_410UOBJS += $(410U_IPCGOO_OBJS)
410UCLEANS += $(410UDIR)/libipcgoo.a

$(410UDIR)/libipcgoo.a: $(410U_IPCGOO_OBJS)
