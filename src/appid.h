/*!
 * \file appid.h
 *
 * \author Copyright (C) 2008 ... 2015 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 *
 * \brief Header file for a DXF application identity symbol table entry
 * (\c APPID).
 *
 * \version The \c APPID table was introduced in DXF R12.
 *
 * A DXF \c APPID entity contains data concerning the application registered
 * with the drawing involved.\n
 * It is a table entry in the \c TABLES section of the DXF file.
 * These table entries maintain a set of names for all applications
 * registered with a drawing.
 *
 * <hr>
 * <h1><b>Copyright Notices.</b></h1>\n
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.\n\n
 * This program is distributed in the hope that it will be useful, but
 * <b>WITHOUT ANY WARRANTY</b>; without even the implied warranty of
 * <b>MERCHANTABILITY</b> or <b>FITNESS FOR A PARTICULAR PURPOSE</b>.\n
 * See the GNU General Public License for more details.\n\n
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to:\n
 * Free Software Foundation, Inc.,\n
 * 59 Temple Place,\n
 * Suite 330,\n
 * Boston,\n
 * MA 02111 USA.\n
 * \n
 * Drawing eXchange Format (DXF) is a defacto industry standard for the
 * exchange of drawing files between various Computer Aided Drafting
 * programs.\n
 * DXF is an industry standard designed by Autodesk(TM).\n
 * For more details see http://www.autodesk.com.
 * <hr>
 */


#ifndef LIBDXF_SRC_APPID_H
#define LIBDXF_SRC_APPID_H


#include "global.h"


/*!
 * \brief Definition of an AutoCAD application identification
 * (\c APPID).
 *
 * This table entry contains a name for an application
 * registered with a drawing.
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12.
 * \version According to DXF R13.
 * \version According to DXF R14.
 */
typedef struct
dxf_appid
{
        int id_code;
                /*!< Identification number for the entity.\n
                 * This is to be an unique (sequential) number in the DXF
                 * file.\n
                 * Group code = 5. */
        char *application_name;
                /*!< Name of the application registered with the drawing.\n
                 * Group code = 2. */
        int flag;
                /*!< This flag is for the benefit of AutoCAD commands;
                 * it can be ignored by most programs that read DXF files,
                 * and need not be set by programs that write DXF files.\n
                 * bit coded:\n
                 * <ol>
                 * <li value = "16"> if set, table entry is externally
                 * dependent on an Xref.</li>
                 * <li value = "32"> if this bit and bit 16 are both set,
                 * the externally dependent Xref has been successfully
                 * resolved.</li>
                 * <li value = "64"> if set, the table entry was
                 * referenced by at least one entity in the drawing the
                 * last time the drawing was edited.</li>
                 * </ol>
                 * Group code = 70. */
        char *dictionary_owner_soft;
                /*!< Soft-pointer ID/handle to owner dictionary
                 * (optional).\n
                 * Group code = 330. */
        char *dictionary_owner_hard;
                /*!< Hard owner ID/handle to owner dictionary
                 * (optional).\n
                 * Group code = 360. */
        struct DxfAppid *next;
                /*!< Pointer to the next DxfAppid.\n
                 * \c NULL in the last DxfAppid. */
} DxfAppid;


DxfAppid *
dxf_appid_new ();
DxfAppid *
dxf_appid_init
(
        DxfAppid *appid
);
DxfAppid *
dxf_appid_read
(
        DxfFile *fp,
        DxfAppid *appid
);
int
dxf_appid_write
(
        DxfFile *fp,
        DxfAppid* appid
);
int
dxf_appid_free
(
        DxfAppid *appid
);
void
dxf_appid_free_chain
(
        DxfAppid *appids
);
int
dxf_appid_is_xreferenced
(
        DxfAppid *appid
);
int
dxf_appid_is_xresolved
(
        DxfAppid *appid
);
int
dxf_appid_is_referenced
(
        DxfAppid *appid
);
int
dxf_appid_is_no_save_xdata
(
        DxfAppid *appid
);


#endif /* LIBDXF_SRC_APPID_H */


/* EOF */
