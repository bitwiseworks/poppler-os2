
set(common_srcs
  parseargs.cc
  Win32Console.cc
)
set(common_libs
  poppler
)

# pdftoppm
set(pdftoppm_SOURCES ${common_srcs}
  pdftoppm.cc
  sanitychecks.cc
)
add_executable(pdftoppm ${pdftoppm_SOURCES})
target_link_libraries(pdftoppm ${common_libs})
if(LCMS2_FOUND)
  target_link_libraries(pdftoppm ${LCMS2_LIBRARIES})
  target_include_directories(pdftoppm SYSTEM PRIVATE ${LCMS2_INCLUDE_DIR})
endif()
if (OS2)
  set_target_properties(pdftoppm PROPERTIES
    OS2_DEF_VENDOR "$ENV{VENDOR}"
    OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
    OS2_DEF_VERSION "${POPPLER_VERSION}")
endif()
install(TARGETS pdftoppm DESTINATION bin)
install(FILES pdftoppm.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

if (HAVE_CAIRO)
  # pdftocairo
  set(pdftocairo_SOURCES ${common_srcs}
    pdftocairo.cc
    pdftocairo-win32.cc
    ${CMAKE_SOURCE_DIR}/poppler/CairoFontEngine.cc
    ${CMAKE_SOURCE_DIR}/poppler/CairoOutputDev.cc
    ${CMAKE_SOURCE_DIR}/poppler/CairoRescaleBox.cc
  )
  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
  )
  add_definitions(${CAIRO_CFLAGS})
  add_executable(pdftocairo ${pdftocairo_SOURCES})
  target_link_libraries(pdftocairo ${CAIRO_LIBRARIES} Freetype::Freetype ${common_libs})
  target_include_directories(pdftocairo SYSTEM PRIVATE ${CAIRO_INCLUDE_DIRS})
  if(LCMS2_FOUND)
    target_link_libraries(pdftocairo ${LCMS2_LIBRARIES})
    target_include_directories(pdftocairo SYSTEM PRIVATE ${LCMS2_INCLUDE_DIR})
  endif()
  if (OS2)
    set_target_properties(pdftocairo PROPERTIES
      OS2_DEF_VENDOR "$ENV{VENDOR}"
      OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
      OS2_DEF_VERSION "${POPPLER_VERSION}")
  endif()
  install(TARGETS pdftocairo DESTINATION bin)
  install(FILES pdftocairo.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
endif ()

# pdfdetach
set(pdfdetach_SOURCES ${common_srcs}
  pdfdetach.cc
)
add_executable(pdfdetach ${pdfdetach_SOURCES})
target_link_libraries(pdfdetach ${common_libs})
if (OS2)
  set_target_properties(pdfdetach PROPERTIES
    OS2_DEF_VENDOR "$ENV{VENDOR}"
    OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
    OS2_DEF_VERSION "${POPPLER_VERSION}")
endif()
install(TARGETS pdfdetach DESTINATION bin)
install(FILES pdfdetach.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

# pdfdetach
set(pdfattach_SOURCES ${common_srcs}
  pdfattach.cc
)
add_executable(pdfattach ${pdfattach_SOURCES})
target_link_libraries(pdfattach ${common_libs})
if (OS2)
  set_target_properties(pdfattach PROPERTIES
    OS2_DEF_VENDOR "$ENV{VENDOR}"
    OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
    OS2_DEF_VERSION "${POPPLER_VERSION}")
endif()
install(TARGETS pdfattach DESTINATION bin)
install(FILES pdfattach.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

# pdffonts
set(pdffonts_SOURCES ${common_srcs}
  pdffonts.cc
)
add_executable(pdffonts ${pdffonts_SOURCES})
target_link_libraries(pdffonts ${common_libs})
if (OS2)
  set_target_properties(pdffonts PROPERTIES
    OS2_DEF_VENDOR "$ENV{VENDOR}"
    OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
    OS2_DEF_VERSION "${POPPLER_VERSION}")
endif()
install(TARGETS pdffonts DESTINATION bin)
install(FILES pdffonts.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

# pdfimages
set(pdfimages_SOURCES ${common_srcs}
  pdfimages.cc
  ImageOutputDev.cc
  ImageOutputDev.h
)
add_executable(pdfimages ${pdfimages_SOURCES})
target_link_libraries(pdfimages ${common_libs})
if (OS2)
  set_target_properties(pdfimages PROPERTIES
    OS2_DEF_VENDOR "$ENV{VENDOR}"
    OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
    OS2_DEF_VERSION "${POPPLER_VERSION}")
endif()
install(TARGETS pdfimages DESTINATION bin)
install(FILES pdfimages.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

# pdfinfo
set(pdfinfo_SOURCES ${common_srcs}
  pdfinfo.cc printencodings.cc
)
add_executable(pdfinfo ${pdfinfo_SOURCES})
target_link_libraries(pdfinfo ${common_libs})
if (OS2)
  set_target_properties(pdfinfo PROPERTIES
    OS2_DEF_VENDOR "$ENV{VENDOR}"
    OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
    OS2_DEF_VERSION "${POPPLER_VERSION}")
endif()
install(TARGETS pdfinfo DESTINATION bin)
install(FILES pdfinfo.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

if (ENABLE_SIGNATURES)
  # pdfsig
  set(pdfsig_SOURCES ${common_srcs}
    pdfsig.cc
  )
  add_executable(pdfsig ${pdfsig_SOURCES})
  target_link_libraries(pdfsig ${common_libs})

  if (OS2)
    set_target_properties(pdfsig PROPERTIES
      OS2_DEF_VENDOR "$ENV{VENDOR}"
      OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
      OS2_DEF_VERSION "${POPPLER_VERSION}")
  endif()

  if (ENABLE_NSS3)
    target_include_directories(pdfsig SYSTEM PRIVATE ${NSS3_INCLUDE_DIRS})
  endif()
  install(TARGETS pdfsig DESTINATION bin)
  install(FILES pdfsig.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
endif ()

# pdftops
set(pdftops_SOURCES ${common_srcs}
  pdftops.cc
  sanitychecks.cc
)
add_executable(pdftops ${pdftops_SOURCES})
target_link_libraries(pdftops ${common_libs})
if(LCMS2_FOUND)
  target_link_libraries(pdftops ${LCMS2_LIBRARIES})
  target_include_directories(pdftops SYSTEM PRIVATE ${LCMS2_INCLUDE_DIR})
endif()
if (OS2)
set_target_properties(pdftops PROPERTIES
    OS2_DEF_VENDOR "$ENV{VENDOR}"
    OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
    OS2_DEF_VERSION "${POPPLER_VERSION}")
endif()
install(TARGETS pdftops DESTINATION bin)
install(FILES pdftops.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

# pdftotext
set(pdftotext_SOURCES ${common_srcs}
  pdftotext.cc printencodings.cc
)
add_executable(pdftotext ${pdftotext_SOURCES})
target_link_libraries(pdftotext ${common_libs})
if (OS2)
set_target_properties(pdftotext PROPERTIES
    OS2_DEF_VENDOR "$ENV{VENDOR}"
    OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
    OS2_DEF_VERSION "${POPPLER_VERSION}")
endif()
install(TARGETS pdftotext DESTINATION bin)
install(FILES pdftotext.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

# pdftohtml
set(pdftohtml_SOURCES ${common_srcs}
  InMemoryFile.cc
  pdftohtml.cc
  HtmlFonts.cc
  HtmlLinks.cc
  HtmlOutputDev.cc
)
add_executable(pdftohtml ${pdftohtml_SOURCES})
target_link_libraries(pdftohtml ${common_libs})
if (OS2)
set_target_properties(pdftohtml PROPERTIES
    OS2_DEF_VENDOR "$ENV{VENDOR}"
    OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
    OS2_DEF_VERSION "${POPPLER_VERSION}")
endif()
install(TARGETS pdftohtml DESTINATION bin)
install(FILES pdftohtml.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

# pdfseparate
set(pdfseparate_SOURCES ${common_srcs}
  pdfseparate.cc
)
add_executable(pdfseparate ${pdfseparate_SOURCES})
target_link_libraries(pdfseparate ${common_libs})
if (OS2)
set_target_properties(pdfseparate PROPERTIES
    OS2_DEF_VENDOR "$ENV{VENDOR}"
    OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
    OS2_DEF_VERSION "${POPPLER_VERSION}")
endif()
install(TARGETS pdfseparate DESTINATION bin)
install(FILES pdfseparate.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

# pdfunite
set(pdfunite_SOURCES ${common_srcs}
  pdfunite.cc
)
add_executable(pdfunite ${pdfunite_SOURCES})
target_link_libraries(pdfunite ${common_libs})
if (OS2)
set_target_properties(pdfunite PROPERTIES
    OS2_DEF_VENDOR "$ENV{VENDOR}"
    OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
    OS2_DEF_VERSION "${POPPLER_VERSION}")
endif()
install(TARGETS pdfunite DESTINATION bin)
install(FILES pdfunite.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

if (OS2)
# text2pdf
set(text2pdf_SOURCES ${common_srcs}
  text2pdf.c
)
add_executable(text2pdf ${text2pdf_SOURCES})
target_link_libraries(text2pdf ${common_libs})
set_target_properties(text2pdf PROPERTIES
    OS2_DEF_VENDOR "$ENV{VENDOR}"
    OS2_DEF_PATCH "${POPPLER_MICRO_VERSION}"
    OS2_DEF_VERSION "${POPPLER_VERSION}")
install(TARGETS text2pdf DESTINATION bin)
endif()
