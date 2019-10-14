# Output Directory
$(SolutionDir)lib\$(Configuration)\$(PlatformTarget)\

# Intermediate Directory
$(SolutionDir)obj\$(Configuration)\$(PlatformTarget)\$(ProjectName)\

# Target Name
ML_$(ProjectName)_$(Configuration)_$(PlatformTarget)

# Working Directory
$(OutDir)

# Additional Include Directories
$(SolutionDir)include
$(SolutionDir)thirdparty\include

# Output File
$(SolutionDir)bin\$(Configuration)\$(PlatformTarget)\$(TargetName)$(TargetExt)

# Additional Library Directories
$(SolutionDir)lib\;$(SolutionDir)lib\$(Configuration)\
$(SolutionDir)lib\$(Configuration)\$(PlatformTarget)\
$(SolutionDir)thirdparty\lib\;$(SolutionDir)thirdparty\lib\$(Configuration)\
$(SolutionDir)thirdparty\lib\$(Configuration)\$(PlatformTarget)\
