## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e674 linker.cmd package/cfg/mutex_pe674.oe674

linker.cmd: package/cfg/mutex_pe674.xdl
	$(SED) 's"^\"\(package/cfg/mutex_pe674cfg.cmd\)\"$""\"D:/Software/macos/SYSBIOS/SYSBIOS/Application/occ_hello/.config/xconfig_mutex/\1\""' package/cfg/mutex_pe674.xdl > $@
