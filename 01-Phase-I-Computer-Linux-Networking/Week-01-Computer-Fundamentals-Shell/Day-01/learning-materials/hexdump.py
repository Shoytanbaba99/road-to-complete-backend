import sys


def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <file>")
        return

    filename = sys.argv[1]

    try:
        with open(filename, "rb") as file:
            offset = 0

            while True:
                buffer = file.read(16)

                if not buffer:
                    break

                # Print offset
                print(f"{offset:08x}: ", end="")

                # Print hexadecimal bytes
                for byte in buffer:
                    print(f"{byte:02x} ", end="")

                # Padding for incomplete final line
                for _ in range(16 - len(buffer)):
                    print("   ", end="")

                # Divider
                print(" | ", end="")

                # Print ASCII representation
                for byte in buffer:
                    if 32 <= byte <= 126:
                        print(chr(byte), end="")
                    else:
                        print(".", end="")

                print()

                offset += len(buffer)

    except FileNotFoundError:
        print(f"Error: file '{filename}' not found.")
    except OSError as e:
        print(f"Error opening file: {e}")


if __name__ == "__main__":
    main()
