import os
import sys

def main() -> int:
    pass

if __name__ == "__main__":
    try:
        exit_code = main()
    except RuntimeError as rt_err:
        print(f"{rt_err}")
        exit_code = 1
    sys.exit(exit_code)