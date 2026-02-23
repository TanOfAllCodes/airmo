#!/bin/bash
set -e

# Navigate to the project directory
cd "$(dirname "$0")"

if [ ! -d "FreeRTOS-Kernel" ]; then
    echo "Cloning FreeRTOS Kernel..."
    git clone https://github.com/FreeRTOS/FreeRTOS-Kernel.git --depth 1
else
    echo "FreeRTOS Kernel already exists."
fi

echo "FreeRTOS Kernel setup complete."
