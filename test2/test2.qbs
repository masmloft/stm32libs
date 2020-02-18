import qbs 1.0

Product {
    type: ["application", "bin"]

    Depends { name:"STM32F103C8T6Module" }

    cpp.includePaths: [
        "CubeMX/Inc",
        "CubeMX/Drivers/STM32F1xx_HAL_Driver/Inc",
        "CubeMX/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy",
        "CubeMX/Drivers/CMSIS/Device/ST/STM32F1xx/Include",
        "CubeMX/Drivers/CMSIS/Include",

        "CubeMX/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc",
        "CubeMX/Middlewares/ST/STM32_USB_Device_Library/Core/Inc",

        "STM32F103C8T6Core",
    ]

    Group
    {
        name: "CubeMX"
        prefix: "CubeMX/"
        excludeFiles: [
            "**/*template*",
            "**/*Template*",
            "Src/main.c",
        ]
        files: [
            "**/*.h",

            "*.ld",
            "*.S",

            //"CubeMX/Drivers/CMSIS/Device/ST/STM32F1xx/*.c",
            "Drivers/STM32F1xx_HAL_Driver/**/*.c",

            "Middlewares/ST/STM32_USB_Device_Library/**/*.c",

            "Src/*.h",
            "Src/*.c",
        ]
    }

    Group
    {
        name: "STM32F103C8T6Core"
        prefix: "STM32F103C8T6Core/"
        files: [
            "**/*.h",
            "**/*.cpp",
        ]
    }

    files: [
        "*.h",
        "*.cpp",
    ]
}

