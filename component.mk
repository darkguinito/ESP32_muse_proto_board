#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

ifdef CONFIG_AUDIO_BOARD_CUSTOM
COMPONENT_ADD_INCLUDEDIRS += ./raspiaudio
COMPONENT_SRCDIRS += ./raspiaudio

COMPONENT_ADD_INCLUDEDIRS += ./raspiaudio_v1_0
COMPONENT_SRCDIRS += ./raspiaudio_v1_0
endif