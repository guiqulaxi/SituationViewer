TEMPLATE = subdirs

SUBDIRS += MapGraphics \
    SituationViewer

SituationViewer.depends += MapGraphics
