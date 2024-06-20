python
import sys
sys.path.insert(0, '/usr/share/gcc-8.2.1/python/')
# This would only enable the printers but not the xmethods:
# from libstdcxx.v6.printers import register_libstdcxx_printers
from libstdcxx.v6 import register_libstdcxx_printers
register_libstdcxx_printers (None)
end 
