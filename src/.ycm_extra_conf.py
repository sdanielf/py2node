import os
import ycm_core

flags = [
    '-Wall',
    '-Wextra',
    '-Werror',
    '-Wc++98-compat',
    '-Wno-long-long',
    '-Wno-variadic-macros',
    '-fexceptions',
    '-DNDEBUG',
    '-DUSE_CLANG_COMPLETER',
    '-std=c++11',
    '-x',
    'c++',
    '-I/usr/include/python2.7',
    '-I/usr/include/x86_64-linux-gnu/python2.7',
]

SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ]

def DirectoryOfThisScript():
  return os.path.dirname( os.path.abspath( __file__ ) )


def MakeRelativePathsInFlagsAbsolute( flags, working_directory ):
  if not working_directory:
    return list( flags )
  new_flags = []
  make_next_absolute = False
  path_flags = [ '-isystem', '-I', '-iquote', '--sysroot=' ]
  for flag in flags:
    new_flag = flag

    if make_next_absolute:
      make_next_absolute = False
      if not flag.startswith( '/' ):
        new_flag = os.path.join( working_directory, flag )

    for path_flag in path_flags:
      if flag == path_flag:
        make_next_absolute = True
        break

      if flag.startswith( path_flag ):
        path = flag[ len( path_flag ): ]
        new_flag = path_flag + os.path.join( working_directory, path )
        break

    if new_flag:
      new_flags.append( new_flag )
  return new_flags



def FindNodeGypPath():

  basepath = os.path.expanduser(os.path.join("~", ".node-gyp"));

  def GetLarger(v1, v2):
    v1_comp = v1.split(".")
    v2_comp = v2.split(".")

    for i in range(0, min(len(v1_comp), len(v2_comp))):
      try:
        comp1 = int(v1_comp[i])
      except:
        comp1 = -1

      try:
        comp2 = int(v2_comp[i])
      except:
        comp1 = -1

      if comp1 < comp2:
        return v2
      if comp1 > comp2:
        return v1

    return v1

  def FindBest(candidates):
    if (len(candidates) == 0):
      return None
    best = candidates[0]

    for candidate in candidates:
      best = GetLarger(best, candidate)

    return os.path.expanduser(os.path.join('~', '.node-gyp', best))

  return FindBest(os.listdir(basepath))


def FlagsForFile( filename, **kwargs ):
  relative_to = DirectoryOfThisScript()

  node_gyp_path = FindNodeGypPath()

  final_flags = flags

  if node_gyp_path:
    final_flags += ["-I", os.path.join(node_gyp_path, "src")]
    final_flags += ["-I", os.path.join(node_gyp_path, "deps", "uv", "include")]
    final_flags += ["-I", os.path.join(node_gyp_path, "deps", "v8", "include")]

  final_flags = MakeRelativePathsInFlagsAbsolute( flags, relative_to )
  print final_flags

  return {
    'flags': final_flags,
    'do_cache': True
  }
