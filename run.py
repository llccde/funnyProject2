import subprocess
# 路径是你的sln文件的绝对路径



SLN_PATH = "E:\\c\\vs\\ctest\\funnyProject\\funnyProject\\funnyProject.sln"
# Visual Studio 2019 的默认安装路径，如果你没改过应该是这个，如果不是需要根据实际情况修改
MSBUILD_PATH = "F:\\vs\\ide\\MSBuild\\Current\\Bin\\MSBuild.exe"
#清理解决方法
#subprocess.run([MSBUILD_PATH, SLN_PATH, "/t:Clean"])
# 这将在子进程中调用MsBuild命令行来构建你的解决方案
subprocess.run([MSBUILD_PATH, SLN_PATH])    
print("hello")
