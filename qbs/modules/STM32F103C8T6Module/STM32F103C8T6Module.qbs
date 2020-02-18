import qbs
import qbs.FileInfo
import qbs.Utilities
import qbs.TextFile
import qbs.Process

Module {
    property string cpu: "cortex-m3"

    Depends { name:"cpp" }
    cpp.positionIndependentCode: false
    cpp.enableExceptions: false
    cpp.enableRtti: undefined
    cpp.visibility: undefined
    cpp.linkerName : "gcc"
    cpp.linkerMode: ""
    cpp.executableSuffix: ".elf"
    cpp.cxxLanguageVersion: "c++11"
    //cpp.debugInformation: false
    //cpp.optimization: "small"

    cpp.defines: [
        "USE_HAL_DRIVER",
        "STM32F103xB",
    ]

    cpp.commonCompilerFlags: [
        //"-Os",
        //"-flto",
        "-Wall",
        "-mthumb",
        "-mcpu=" + cpu,
        "-fdata-sections",
        "-ffunction-sections",
    ]

    cpp.assemblerFlags: [
        "-x",
        "assembler-with-cpp",
    ]

    cpp.cFlags: [
        "-MMD",
        "-MP",
    ]

    cpp.cxxFlags: [
        "-MMD",
        "-MP",
    ]

    cpp.linkerFlags:[
        //"-flto",
        "-mthumb",
        "-mcpu=" + cpu,
        //"-msoft-float",
        //"-mfpu=vfp",
        "--specs=nano.specs",
        "--specs=nosys.specs",
        //"-Wl,-Map=" + product.buildDirectory + "/" + product.name + ".map,--cref",
        "-Wl,--gc-sections",
        //"-T" + path + "/STM32F103C8Tx_FLASH.ld",
        "-lc",
        "-lm",
        "-lnosys",
        //"-lgcc",
        //"-lstdc++",
    ]

    Properties {
        condition: qbs.buildVariant === "debug"
        //cpp.defines: outer.concat(["DEBUG=1"])
        //cpp.cFlags: outer.concat(["-g", "-gdwarf-2"])
    }

    Properties {
        condition: qbs.buildVariant === "release"
        cpp.optimization: "small"
//        cpp.commonCompilerFlags: outer.concat(["-flto"])
//        cpp.linkerFlags: outer.concat(["-flto"])
        //cpp.cFlags: outer.concat(["-flto"])
    }

    FileTagger {
        patterns: "*.ld"
        fileTags: ["linkerscript"]
    }

//    cpp.shouldLink:false

//    Rule {
//        name: "staticLibraryLinker"
//        condition: true
//        multiplex: true
//        inputs: ["obj", "linkerscript"]
//        inputsFromDependencies: ["staticlibrary"]
//        Artifact {
//            fileTags: ["staticlibrary"]
//            filePath: product.name + ".a"
//        }
//        prepare: {
//            var args = ['rcs', output.filePath];
//            for (var i in inputs.obj)
//                args.push(inputs.obj[i].filePath);
//            var cmd = new Command(product.cpp.archiverPath, args);
//            cmd.description = 'Creating ' + output.fileName;
//            cmd.highlight = 'linker'
//            cmd.responseFileUsagePrefix = '@';
//            return cmd;
//        }
//    }

//    Rule {
//        name: "applicationLinker"
//        multiplex: true
//        inputs: ["obj", "linkerscript"]
//        inputsFromDependencies: ["staticlibrary"]
//        Artifact {
//            fileTags: ["application"]
//            filePath: product.name + ".elf"
//        }
//        prepare: {
//            var compilers = product.cpp.compilerPathByLanguage;
//            var linkerPath = compilers["c"]

//            var args = [];

//            args = args.concat(product.cpp.linkerFlags)
//            args = args.concat(product.cpp.driverFlags)

//            args.push('-o');
//            args.push(output.filePath);

//            for (i in inputs.linkerscript)
//                args.push("-T" + inputs.linkerscript[i].filePath);

//            if(product.cpp.libraryPaths)
//            {
//                for (i in product.cpp.libraryPaths)
//                    args.push("-L" + product.cpp.libraryPaths[i]);
//            }

//            args.push("-Wl,--start-group");

//            for (i in inputs["obj"])
//                args.push(inputs["obj"][i].filePath);

//            if(inputs.staticlibrary)
//            {
//                for (i in inputs.staticlibrary)
//                    args.push(inputs.staticlibrary[i].filePath);
//            }

//            for (i in product.cpp.staticLibraries)
//                args.push("-l" + product.cpp.staticLibraries[i]);

//            args.push("-Wl,--end-group");


//            var cmd = new Command(linkerPath , args);
//            cmd.description = 'Linking ' + product.name;
//            cmd.highlight = "linker";

//            return [cmd];
//        }
//    }


    Rule
    {
        id: binGen
        inputs: ["application"]
        Artifact
        {
            filePath: product.name + ".bin"
            fileTags: "bin"
        }
        prepare:
        {
            var binArgs = ["-O", "binary", "-S", input.filePath, output.filePath];
            var binApp = product.moduleProperty("cpp", "objcopyPath")
            var binCmd = new Command(binApp, binArgs);
            binCmd.description = "Generate " + output.fileName;
            binCmd.highlight = "codegen";

            var sizeArgs = [input.filePath];
            var sizeApp = product.moduleProperty("cpp", "toolchainPathPrefix") + "size";
            var sizeCmd = new Command(sizeApp, sizeArgs);
            sizeCmd.description = "Size: " + input.fileName
            sizeCmd.highlight = "codegen";

            return [binCmd, sizeCmd];
        }
    }
}
