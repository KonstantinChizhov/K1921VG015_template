import os
import datetime

env = Environment()
env["ENV"] = os.environ

env["FLASH_TOOL_PATH"] = "C:/tools/openocd_Patch_Niiet/"

env["FLASH_TOOL"] = f'{env["FLASH_TOOL_PATH"]}/openocd.exe'
env["OCD_INTERFACE"] = "interface/ftdi/vg015_dev_onboard_ftdi.cfg"
env["OCD_TARGET"] = "target/k1921vg015.cfg"

target_arch = "rv32imfc_zicsr_zifencei"
target_abi = "ilp32f"

if not env.Detect("riscv-none-elf-gcc"):
    raise Exception("riscv-none-elf-gcc not found")
# env["TOOL_PREFIX"] = "riscv-none-elf-"
env["TOOL_PREFIX"] = (
    "C:/Users/Konstantin/Downloads/sc-dt/riscv-gcc/bin/riscv64-unknown-elf-"
)

prefix = env["TOOL_PREFIX"]
env["CC"] = prefix + "gcc"
env["CXX"] = prefix + "g++"
env["AR"] = prefix + "ar"
env["AS"] = prefix + "gcc"
env["LINK"] = prefix + "gcc"
env["OBJCOPY"] = prefix + "objcopy"
env["OBJDUMP"] = prefix + "objdump"
env["NM"] = prefix + "nm"
env["RANLIB"] = prefix + "ranlib"
env["SIZE"] = prefix + "size"
env["PROGSUFFIX"] = ".elf"
env["GDB"] = prefix + "gdb"


hexBuilder = Builder(
    action="$OBJCOPY -O ihex --only-section .isr_vectors --only-section .text --only-section .rodata --only-section .ctors --only-section .dtors --only-section .data --only-section .metadata_section $CUSTOM_HEX_PARAMS $SOURCE $TARGET",
    src_suffix=".elf",
    suffix=".hex",
)

binBuilder = Builder(
    action="$OBJCOPY -O binary --only-section $SECTION_NAME $SOURCE $TARGET",
    src_suffix=".elf",
    suffix=".bin",
)

disasmBuilder = Builder(
    action="$OBJDUMP -h -S $SOURCE > $TARGET", src_suffix=".elf", suffix=".lss"
)


def openOcdFlashImage(target, source, env):
    command = '"$FLASH_TOOL" '
    script_path = "scripts"
    if os.path.isdir(os.path.join(env["FLASH_TOOL_PATH"], script_path)):
        command += f'-s "%(FLASH_TOOL_PATH)s{script_path}" '
    src_path = source[0].abspath.replace("\\", "/")

    command += "-f %(OCD_INTERFACE)s "
    # command += '-c "transport select jtag" '
    command += "-f %(OCD_TARGET)s "
    command += '-c "reset_config trst_only" '
    command += '-c "adapter speed 1500" '
    command += '-c "init" -c "reset init" '
    command += '-c "flash probe 0" '
    command += f'-c "program {src_path}" -c "reset" -c "exit"'

    command = command % env
    res = env.Execute(command)
    logFile = open(target[0].abspath, "w")
    logFile.write(
        "Flash tool executed\r\n%s\r\n%s\r\nResult: %s\n"
        % (command, datetime.datetime.now(), res)
    )
    return res


flashBuilder = Builder(action=openOcdFlashImage, src_suffix=".elf", suffix="_flash.log")

env.Append(
    BUILDERS={
        "Hex": hexBuilder,
        "Disassembly": disasmBuilder,
        "DumpSection": binBuilder,
        "Flash": flashBuilder,
    }
)

env.Append(
    CCFLAGS=[
        f"-march={target_arch}",
        f"-mabi={target_abi}",
        "-msmall-data-limit=8",
        "-mstrict-align",
        "-mno-save-restore",
        "-Og",
        "-fmessage-length=0",
        "-fsigned-char",
        "-ffunction-sections",
        "-fdata-sections",
        "-fno-common",
        "-fno-builtin",
        # "-flto",
        "-Wall",
        "-Wextra",
        "-ffreestanding",
        "-g3",
        "-ggdb",
        "--specs=nano.specs",
        "-nostdlib",
    ]
)

env.Append(
    CPPDEFINES={
        "F_OSC": 16000000,
        "__STDC_HOSTED__": "1",
    }
)

env["ASFLAGS"] = [
    f"-march={target_arch}",
    f"-mabi={target_abi}",
    "-xassembler-with-cpp",
    f"-I{env.Dir('#/inc').srcnode().abspath}",
    "-fno-common",
    "-fno-builtin",
    "-c",
]

env["CXXFLAGS"] = [
    "-fno-exceptions",
    "-fno-threadsafe-statics",
    "-fno-rtti",
    "-fuse-cxa-atexit",
    "-Woverloaded-virtual",
    "-std=c++20",
    "-fno-use-cxa-atexit",
]

env.Append(CPPPATH=["inc"])

formattedLinkerScript = "#/ldscripts/k1921vg015_flash.ld"
linkerscript = env.File(formattedLinkerScript).srcnode().abspath
linkerscript = "-T%s" % linkerscript

env.Append(
    LINKFLAGS=[
        "-Wl,--gc-sections",
        "-Wl,-Map=${TARGET.base}.map",
        "-g3",
        "-ggdb",
        linkerscript,
        "-L" + env.Dir("#/ldscripts").srcnode().abspath,
        "-nostartfiles",
        "-Xlinker",
        "-lgcc",
        "-lc",
        f"-march={target_arch}",
        f"-mabi={target_abi}",
        "-fno-use-cxa-atexit",
        "--specs=nano.specs",
    ]
)


def ObjectName(srcName):
    srcFilename = os.path.splitext(os.path.basename(srcName))[0]
    return os.path.join("#./build", f"{srcFilename}.o")


platform_sources = [
    "#/src/platform/startup.c",
    "#/src/platform/plic.c",
    "#/src/platform/debug.cpp",
]

cpp_sources = [f.srcnode().abspath for f in env.Glob("#/src/*.cpp")]
c_sources = [f.srcnode().abspath for f in env.Glob("#/src/*.c")]

objects = [
    env.Object(ObjectName(src), src)
    for src in platform_sources + cpp_sources + c_sources
]

elf = env.Program("test.elf", objects)
hex = env.Hex("test.hex", elf)
lss = env.Disassembly("test.lss", elf)

env.Flash(elf)
