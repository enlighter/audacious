/*
 * about.cc
 * Copyright 2014 William Pitcock
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    provided with the distribution.
 *
 * This software is provided "as is" and without any warranty, express or
 * implied. In no event shall the authors be liable for any damages arising from
 * the use of this software.
 */

#include <QDialog>
#include <QFile>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QTextStream>
#include <QVBoxLayout>

#include <libaudcore/audstrings.h>
#include <libaudcore/i18n.h>
#include <libaudcore/runtime.h>

#include "libaudqt.h"

static QTabWidget * buildCreditsNotebook (QWidget * parent)
{
    const char * data_dir = aud_get_path (AudPath::DataDir);
    const char * titles[2] = {_("Credits"), _("License")};
    const char * filenames[2] = {"AUTHORS", "COPYING"};

    auto tabs = new QTabWidget (parent);

    for (int i = 0; i < 2; i++)
    {
        QFile f (QString (filename_build ({data_dir, filenames[i]})));
        if (!f.open (QIODevice::ReadOnly))
            continue;

        QTextStream in(&f);

        auto edit = new QPlainTextEdit (in.readAll(), parent);
        edit->setReadOnly (true);
        tabs->addTab (edit, titles[i]);

        f.close();
    }

    return tabs;
}

static QDialog * buildAboutWindow ()
{
    const char * data_dir = aud_get_path (AudPath::DataDir);
    const char * logo_path = filename_build ({data_dir, "images", "about-logo.png"});
    const char about_text[] =
     "<big><b>Audacious " VERSION "</b></big><br>\n"
     "Copyright © 2001-2014 Audacious developers and others";

    auto window = new QDialog;

    auto logo = new QLabel (window);
    logo->setPixmap (QPixmap (logo_path));
    logo->setAlignment (Qt::AlignHCenter);

    auto text = new QLabel (about_text, window);
    text->setAlignment (Qt::AlignHCenter);

    auto layout = new QVBoxLayout (window);
    layout->addWidget (logo);
    layout->addWidget (text);
    layout->addWidget (buildCreditsNotebook (window));

    window->setWindowTitle (_("About Audacious"));
    window->resize (640, 480);

    return window;
}

static QDialog *s_aboutwin = nullptr;

namespace audqt {

EXPORT void aboutwindow_show ()
{
    if (!s_aboutwin)
        s_aboutwin = buildAboutWindow ();

    window_bring_to_front (s_aboutwin);
}

EXPORT void aboutwindow_hide ()
{
    if (!s_aboutwin)
        return;

    s_aboutwin->hide ();
}

}
