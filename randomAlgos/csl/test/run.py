import argparse
import numpy as np
from cerebras.sdk.runtime.sdkruntimepybind import SdkRuntime, MemcpyDataType, MemcpyOrder

parser = argparse.ArgumentParser()
parser.add_argument('--name', help='the output directory from compilation')
args = parser.parse_args()

runner = SdkRuntime(args.name)

A_sym = runner.get_id('A')
B_sym = runner.get_id('B')
C_sym = runner.get_id('C')

runner.load()
runner.run()

data_A = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], dtype=np.float32)
data_B = np.array([10, 20, 30, 40, 50, 60, 70, 80, 90, 100], dtype=np.float32)

runner.memcpy_h2d(A_sym, data_A, 0, 0, 1, 1, 10, streaming=False,
                  data_type=MemcpyDataType.MEMCPY_32BIT,
                  order=MemcpyOrder.COL_MAJOR, nonblock=False)

runner.memcpy_h2d(B_sym, data_B, 0, 0, 1, 1, 10, streaming=False,
                  data_type=MemcpyDataType.MEMCPY_32BIT,
                  order=MemcpyOrder.COL_MAJOR, nonblock=False)

print("run")
runner.launch("add_arrays", nonblock=False)
result = np.zeros(10, dtype=np.float32)
runner.memcpy_d2h(result, C_sym, 0, 0, 1, 1, 10, streaming=False, 
                  data_type=MemcpyDataType.MEMCPY_32BIT, order=MemcpyOrder.COL_MAJOR, nonblock=False)
runner.stop()

print(f"result: {result}")
