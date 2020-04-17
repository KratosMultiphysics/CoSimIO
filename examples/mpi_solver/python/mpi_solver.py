try:
    import mpi4py
except ImportError:
    print('"mpi4py is not available')
    sys.exit(1)