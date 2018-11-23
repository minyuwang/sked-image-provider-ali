TEMPLATE = subdirs

SUBDIRS = lib
example {
  SUBDIRS += example
  example.depends += lib
}
