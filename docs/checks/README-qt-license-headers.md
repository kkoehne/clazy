# qt-license-headers

Prints the set of Qt licenses an #include does - directly or indirectly - pull in.

Some Qt modules are available only under GPLv3 license. In this case, Qt headers will contain
the license and a marker like

  $QT_BEGIN_LICENSE:GPL$

This check will scan all direct or indirect include files for this marker, and will print all
found licenses per #include.

This check is disabled by default and must be explicitly enabled.

