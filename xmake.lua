


--此文件仅用于生成complie_commands
--[[
toolchain("riscv-none-embed")
    set_kind("standalone")
    set_sdkdir("C:/MounRiver/MounRiver_Studio/toolchain/RISC-V Embedded GCC")
    set_toolset("cc", "riscv-none-embed-gcc.exe")
    set_toolset("cxx", "riscv-none-embed-gcc.exe")
    set_toolset("ld", "riscv-none-embed-ld.exe")
    set_toolset("sh", "riscv-none-embed-gcc.exe", "riscv-none-embed-gcc.exe")
    set_toolset("ar", "riscv-none-embed-ar.exe")
    set_toolset("ex", "riscv-none-embed-ar.exe")
    set_toolset("strip", "riscv-none-embed-strip.exe")
    set_toolset("mm", "riscv-none-embed-gcc.exe")
    set_toolset("mxx", "riscv-none-embed-gcc.exe", "riscv-none-embed-g++.exe")
    set_toolset("as", "riscv-none-embed-gcc.exe")
    on_load(function (toolchain)
        toolchain:add("cxflags", "-march=rv32imac", "-mabi=ilp32", 
            "-msmall-data-limit=8", "-mno-save-restore", "-Os", 
            "-fmessage-length=0", "-fsigned-char", "-ffunction-sections", 
            "-fdata-sections", "-Wunused", "-Wuninitialized", 
            "-g")
        toolchain:add("ldflags", "-nostartfiles", "--gc-sections")
        toolchain:add("shflags", toolchain:get("ldflags"))
        toolchain:add("scflags", toolchain:get("cxflags"))
        toolchain:add("links", "c") -- add other libraries if needed
    end)
toolchain_end()--]]

target("car_23")
    set_kind("binary")
    add_includedirs("Libraries/hefei_libraries", "./src/USER/inc"
        , "./src/CODE", "Libraries/Device/Core", "Libraries/hefei_libraries/board"
        , "Libraries/hefei_peripheral", "Libraries/Device/CH32V30x_p/CH32V30x_Lib")
    add_files("./**.c")
    after_build(function (target)
        os.mv("startfiles", "10-Car_CAM_uart_PC.elf")
    end)