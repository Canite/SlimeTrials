def Settings( **kwargs ):
  return {
    'flags': [
        '-std=c99',
        "-D__TARGET_gb",
        '-D__PORT_sm83',
        "-D__banked",
        "-D__naked",
        '-I/home/canight/gbdk/include',
        '-L/home/canight/gbdk/lib',
        '-I./include',
    ],
  }
