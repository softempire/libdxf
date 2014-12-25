/*!
 * \file trace.c
 *
 * \author Copyright (C) 2008 ... 2014 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 *
 * \brief DXF trace entity (\c TRACE).
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
 * For more details see http://www.autodesk.com .
 * <hr>
 */


#include "trace.h"


/*!
 * \brief Allocate memory for a DXF \c TRACE.
 *
 * Fill the memory contents with zeros.
 *
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 */
DxfTrace *
dxf_trace_new ()
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        DxfTrace *dxf_trace = NULL;
        size_t size;

        size = sizeof (DxfTrace);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((dxf_trace = malloc (size)) == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfTrace struct.\n")),
                  __FUNCTION__);
                dxf_trace = NULL;
        }
        else
        {
                memset (dxf_trace, 0, size);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (dxf_trace);
}


/*!
 * \brief Allocate memory and initialize data fields in a DXF \c TRACE
 * entity.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 */
DxfTrace *
dxf_trace_init
(
        DxfTrace *dxf_trace
                /*!< DXF trace entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dxf_trace == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                dxf_trace = dxf_trace_new ();
        }
        if (dxf_trace == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfTrace struct.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        dxf_trace->id_code = 0;
        dxf_trace->linetype = strdup (DXF_DEFAULT_LINETYPE);
        dxf_trace->layer = strdup (DXF_DEFAULT_LAYER);
        dxf_trace->x0 = 0.0;
        dxf_trace->y0 = 0.0;
        dxf_trace->z0 = 0.0;
        dxf_trace->x1 = 0.0;
        dxf_trace->y1 = 0.0;
        dxf_trace->z1 = 0.0;
        dxf_trace->x2 = 0.0;
        dxf_trace->y2 = 0.0;
        dxf_trace->z2 = 0.0;
        dxf_trace->x3 = 0.0;
        dxf_trace->y3 = 0.0;
        dxf_trace->z3 = 0.0;
        dxf_trace->elevation = 0.0;
        dxf_trace->thickness = 0.0;
        dxf_trace->linetype_scale = DXF_DEFAULT_LINETYPE_SCALE;
        dxf_trace->visibility = DXF_DEFAULT_VISIBILITY;
        dxf_trace->color = DXF_COLOR_BYLAYER;
        dxf_trace->paperspace = DXF_MODELSPACE;
        dxf_trace->extr_x0 = 0.0;
        dxf_trace->extr_y0 = 0.0;
        dxf_trace->extr_z0 = 0.0;
        dxf_trace->dictionary_owner_soft = strdup ("");
        dxf_trace->dictionary_owner_hard = strdup ("");
        dxf_trace->next = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (dxf_trace);
}


/*!
 * \brief Write DXF output to fp for a DCF \c TRACE entity.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_trace_write
(
        DxfFile *fp,
                /*!< DXF file pointer to an output file (or device). */
        DxfTrace *dxf_trace
                /*!< DXF trace entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        char *dxf_entity_name = strdup ("TRACE");

        /* Do some basic checks. */
        if (dxf_trace == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (strcmp (dxf_trace->linetype, "") == 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () empty linetype string for the %s entity with id-code: %x\n")),
                  __FUNCTION__, dxf_entity_name, dxf_trace->id_code);
                fprintf (stderr,
                  (_("\t%s entity is reset to default linetype")),
                  dxf_entity_name);
                dxf_trace->linetype = strdup (DXF_DEFAULT_LINETYPE);
        }
        if (strcmp (dxf_trace->layer, "") == 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () empty layer string for the %s entity with id-code: %x\n")),
                  __FUNCTION__, dxf_entity_name, dxf_trace->id_code);
                fprintf (stderr,
                  (_("\t%s entity is relocated to layer 0")),
                  dxf_entity_name);
                dxf_trace->layer = strdup (DXF_DEFAULT_LAYER);
        }
        /* Start writing output. */
        fprintf (fp->fp, "  0\n%s\n", dxf_entity_name);
        if (dxf_trace->id_code != -1)
        {
                fprintf (fp->fp, "  5\n%x\n", dxf_trace->id_code);
        }
        if (strcmp (dxf_trace->linetype, DXF_DEFAULT_LINETYPE) != 0)
        {
                fprintf (fp->fp, "  6\n%s\n", dxf_trace->linetype);
        }
        fprintf (fp->fp, "  8\n%s\n", dxf_trace->layer);
        fprintf (fp->fp, " 10\n%f\n", dxf_trace->x0);
        fprintf (fp->fp, " 20\n%f\n", dxf_trace->y0);
        fprintf (fp->fp, " 30\n%f\n", dxf_trace->z0);
        fprintf (fp->fp, " 11\n%f\n", dxf_trace->x1);
        fprintf (fp->fp, " 21\n%f\n", dxf_trace->y1);
        fprintf (fp->fp, " 31\n%f\n", dxf_trace->z1);
        fprintf (fp->fp, " 12\n%f\n", dxf_trace->x2);
        fprintf (fp->fp, " 22\n%f\n", dxf_trace->y2);
        fprintf (fp->fp, " 32\n%f\n", dxf_trace->z2);
        fprintf (fp->fp, " 13\n%f\n", dxf_trace->x3);
        fprintf (fp->fp, " 23\n%f\n", dxf_trace->y3);
        fprintf (fp->fp, " 33\n%f\n", dxf_trace->z3);
        if (dxf_trace->thickness != 0.0)
        {
                fprintf (fp->fp, " 39\n%f\n", dxf_trace->thickness);
        }
        if (dxf_trace->color != DXF_COLOR_BYLAYER)
        {
                fprintf (fp->fp, " 62\n%d\n", dxf_trace->color);
        }
        if (dxf_trace->paperspace == DXF_PAPERSPACE)
        {
                fprintf (fp->fp, " 67\n%d\n", DXF_PAPERSPACE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a DXF \c TRACE and all it's
 * data fields.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_trace_free
(
        DxfTrace *dxf_trace
                /*!< Pointer to the memory occupied by the DXF \c TRACE
                 * entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        if (dxf_trace->next != NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () pointer to next DxfTrace was not NULL.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        free (dxf_trace->linetype);
        free (dxf_trace->layer);
        free (dxf_trace->dictionary_owner_soft);
        free (dxf_trace->dictionary_owner_hard);
        free (dxf_trace);
        dxf_trace = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/* EOF */
