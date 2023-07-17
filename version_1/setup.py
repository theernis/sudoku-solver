from distutils.core import setup, Extension


def main():
    setup(
        name="solver",
        version="1.0.0",
        description="what's description?",
        ext_modules=[Extension("solver", ["solver.c"])],  # extra_compile_args=["-O2"]
    )


if __name__ == "__main__":
    main()