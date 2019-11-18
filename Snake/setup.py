from distutils.core import setup, Extension
from glob import glob

qt = 'C:/Qt/5.9.7/msvc2017_64'
# qt_libs = 'C:/Qt/5.12.2/msvc2017_64/lib/'
anaconda = 'C:/Users/Victor/Anaconda3/envs/GYM_ENV'

#qt = 'E:/Qt5.12/5.12.2/msvc2017_64'
#anaconda = 'F:/Anaconda/envs/dl3'


module1 = Extension('pySnake',
                    include_dirs = glob(anaconda + '/include')
                    + glob(qt + '/include/')
                    + glob(qt + '/include/QtGui')
                    + glob(qt + '/include/QtWidgets')
                    + glob(qt + '/include/QtCore'),
                    libraries = ['python36', 'Qt5Gui', 'Qt5Widgets', 'Qt5Core'],
                    library_dirs = [anaconda + '/libs', qt + '/lib/'],
                    sources = glob('*.cpp') + glob('./moc_files/*.cpp'))

setup (name = 'PackageName',
       version = '1.0',
       ext_modules = [module1])