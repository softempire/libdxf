/*!
 * \file spatial_filter.c
 *
 * \author Copyright (C) 2015 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 *
 * \brief Functions for a DXF spatial_filter object (\c SPATIAL_FILTER).
 *
 * \version The \c SPATIAL_FILTER object was introduced in DXF R14.
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


#include "spatial_filter.h"


/*!
 * \brief Allocate memory for a \c DxfSpatialFilter.
 *
 * Fill the memory contents with zeros.
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12 (backward compatibility).
 * \version According to DXF R13 (backward compatibility).
 * \version According to DXF R14.
 */
DxfSpatialFilter *
dxf_spatial_filter_new ()
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        DxfSpatialFilter *spatial_filter = NULL;
        size_t size;

        size = sizeof (DxfSpatialFilter);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((spatial_filter = malloc (size)) == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfSpatialFilter struct.\n")),
                  __FUNCTION__);
                spatial_filter = NULL;
        }
        else
        {
                memset (spatial_filter, 0, size);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (spatial_filter);
}


/*!
 * \brief Allocate memory and initialize data fields in a \c SPATIAL_FILTER
 * object.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12 (backward compatibility).
 * \version According to DXF R13 (backward compatibility).
 * \version According to DXF R14.
 */
DxfSpatialFilter *
dxf_spatial_filter_init
(
        DxfSpatialFilter *spatial_filter
                /*!< a pointer to the DXF \c SPATIAL_FILTER object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        int i;

        /* Do some basic checks. */
        if (spatial_filter == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                spatial_filter = dxf_spatial_filter_new ();
        }
        if (spatial_filter == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfSpatialFilter struct.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        spatial_filter->id_code = 0;
        spatial_filter->dictionary_owner_soft = strdup ("");
        spatial_filter->dictionary_owner_hard = strdup ("");
        spatial_filter->x1 = 0.0;
        spatial_filter->y1 = 0.0;
        spatial_filter->z1 = 0.0;
        spatial_filter->front_clipping_plane_distance = 0.0;
        spatial_filter->back_clipping_plane_distance = 0.0;
        spatial_filter->number_of_points = 0;
        spatial_filter->clip_boundary_display_flag = 0;
        spatial_filter->front_clipping_plane_flag = 0;
        spatial_filter->back_clipping_plane_flag = 0;
        spatial_filter->extr_x0 = 0.0;
        spatial_filter->extr_y0 = 0.0;
        spatial_filter->extr_z0 = 0.0;
        for (i = 0; i < 12; i++)
        {
                spatial_filter->inverse_block_transformation[i] = 0.0;
                spatial_filter->block_transformation[i] = 0.0;
        }
        for (i = 0; i < DXF_MAX_PARAM; i++)
        {
                spatial_filter->x0[i] = 0.0;
                spatial_filter->y0[i] = 0.0;
        }
        spatial_filter->next = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (spatial_filter);
}


/*!
 * \brief Read data from a DXF file into a DXF \c SPATIAL_FILTER object.
 *
 * The last line read from file contained the string "SPATIAL_FILTER". \n
 * Now follows some data for the \c SPATIAL_FILTER, to be terminated with a
 * "  0" string announcing the following object, or the end of the
 * \c OBJECTS section marker \c ENDSEC. \n
 * While parsing the DXF file store data in \c spatial_filter. \n
 *
 * \return a pointer to \c spatial_filter.
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12 (backward compatibility).
 * \version According to DXF R13 (backward compatibility).
 * \version According to DXF R14.
 */
DxfSpatialFilter *
dxf_spatial_filter_read
(
        DxfFile *fp,
                /*!< DXF file pointer to an input file (or device). */
        DxfSpatialFilter *spatial_filter
                /*!< a pointer to the DXF \c SPATIAL_FILTER object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        char *temp_string = NULL;
        int i;
        int j;
        int k;

        /* Do some basic checks. */
        if (fp == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL file pointer was passed.\n")),
                  __FUNCTION__);
                /* Clean up. */
                free (temp_string);
                return (NULL);
        }
        if (fp->acad_version_number < AutoCAD_14)
        {
                fprintf (stderr,
                  (_("Warning in %s () illegal DXF version for this entity.\n")),
                  __FUNCTION__);
        }
        if (spatial_filter == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                spatial_filter = dxf_spatial_filter_new ();
                spatial_filter = dxf_spatial_filter_init (spatial_filter);
        }
        i = 0;
        j = 0;
        k = 0;
        (fp->line_number)++;
        fscanf (fp->fp, "%[^\n]", temp_string);
        while (strcmp (temp_string, "0") != 0)
        {
                if (ferror (fp->fp))
                {
                        fprintf (stderr,
                          (_("Error in %s () while reading from: %s in line: %d.\n")),
                          __FUNCTION__, fp->filename, fp->line_number);
                        /* Clean up. */
                        free (temp_string);
                        fclose (fp->fp);
                        return (NULL);
                }
                if (strcmp (temp_string, "5") == 0)
                {
                        /* Now follows a string containing a sequential
                         * id number. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%x\n", &spatial_filter->id_code);
                }
                else if (strcmp (temp_string, "10") == 0)
                {
                        /* Now follows a string containing the
                         * X-value of the clip boundary definition point. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &spatial_filter->x0[i]);
                        i++;
                }
                else if (strcmp (temp_string, "11") == 0)
                {
                        /* Now follows a string containing the X-value
                         * of the origin used to define the local
                         * coordinate system of the clip boundary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &spatial_filter->x1);
                }
                else if (strcmp (temp_string, "20") == 0)
                {
                        /* Now follows a string containing the
                         * Y-value of the clip boundary definition point. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &spatial_filter->y0[j]);
                        j++;
                }
                else if (strcmp (temp_string, "21") == 0)
                {
                        /* Now follows a string containing the Y-value
                         * of the origin used to define the local
                         * coordinate system of the clip boundary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &spatial_filter->y1);
                }
                else if (strcmp (temp_string, "31") == 0)
                {
                        /* Now follows a string containing the Z-value
                         * of the origin used to define the local
                         * coordinate system of the clip boundary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &spatial_filter->z1);
                }
                else if ((strcmp (temp_string, "40") == 0)
                  && (k == 0)
                  && (spatial_filter->front_clipping_plane_flag == 1))
                {
                        /* Now follows a string containing the front
                         * clipping plane distance. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &spatial_filter->front_clipping_plane_distance);
                        k++;
                }
                else if ((strcmp (temp_string, "40") == 0)
                  && (k > 0)
                  && (k < 13)) /* k = 1 ... 12. */
                {
                        /* Now follows a string containing an element of
                         * the 4x3 inverse transformation matrix. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &spatial_filter->inverse_block_transformation[(k - 1)]);
                        k++;
                }
                else if ((strcmp (temp_string, "40") == 0)
                  && (k >= 13)) /* k = 13 ...25. */
                {
                        /* Now follows a string containing an element of
                         * the 4x3 transformation matrix. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &spatial_filter->block_transformation[(k - 13)]);
                        k++;
                }
                else if ((strcmp (temp_string, "41") == 0)
                  && (spatial_filter->back_clipping_plane_flag == 1))
                {
                        /* Now follows a string containing the back
                         * clipping plane distance. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &spatial_filter->back_clipping_plane_distance);
                        i++;
                }
                else if (strcmp (temp_string, "70") == 0)
                {
                        /* Now follows a string containing the
                         * number of points. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &spatial_filter->number_of_points);
                }
                else if (strcmp (temp_string, "71") == 0)
                {
                        /* Now follows a string containing the clip
                         * boundary display enabled flag. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &spatial_filter->clip_boundary_display_flag);
                }
                else if (strcmp (temp_string, "72") == 0)
                {
                        /* Now follows a string containing the front
                         * clipping plane flag. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &spatial_filter->front_clipping_plane_flag);
                }
                else if (strcmp (temp_string, "73") == 0)
                {
                        /* Now follows a string containing the back
                         * clipping plane flag. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &spatial_filter->back_clipping_plane_flag);
                }
                else if ((fp->acad_version_number >= AutoCAD_13)
                        && (strcmp (temp_string, "100") == 0))
                {
                        /* Now follows a string containing the
                         * subclass marker value. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", temp_string);
                        if ((strcmp (temp_string, "AcDbFilter") != 0)
                          && (strcmp (temp_string, "AcDbSpatialFilter") != 0))
                        {
                                fprintf (stderr,
                                  (_("Warning in %s () found a bad subclass marker in: %s in line: %d.\n")),
                                  __FUNCTION__, fp->filename, fp->line_number);
                        }
                }
                else if (strcmp (temp_string, "210") == 0)
                {
                        /* Now follows a string containing the X-value
                         * of the normal to the plane containing the
                         * clip boundary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &spatial_filter->extr_x0);
                }
                else if (strcmp (temp_string, "220") == 0)
                {
                        /* Now follows a string containing the Y-value
                         * of the normal to the plane containing the
                         * clip boundary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &spatial_filter->extr_y0);
                }
                else if (strcmp (temp_string, "230") == 0)
                {
                        /* Now follows a string containing the
                         * Z-value of the extrusion vector. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &spatial_filter->extr_z0);
                }
                else if (strcmp (temp_string, "330") == 0)
                {
                        /* Now follows a string containing the Z-value
                         * of the normal to the plane containing the
                         * clip boundary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", spatial_filter->dictionary_owner_soft);
                        i++;
                }
                else if (strcmp (temp_string, "360") == 0)
                {
                        /* Now follows a string containing Hard owner
                         * ID/handle to owner dictionary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", spatial_filter->dictionary_owner_hard);
                }
                else if (strcmp (temp_string, "999") == 0)
                {
                        /* Now follows a string containing a comment. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", temp_string);
                        fprintf (stdout, (_("DXF comment: %s\n")), temp_string);
                }
                else
                {
                        fprintf (stderr,
                          (_("Warning in %s () unknown string tag found while reading from: %s in line: %d.\n")),
                          __FUNCTION__, fp->filename, fp->line_number);
                }
        }
        /* Clean up. */
        free (temp_string);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (spatial_filter);
}


/*!
 * \brief Write DXF output to a file for a DXF \c SPATIAL_FILTER object.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12 (backward compatibility).
 * \version According to DXF R13 (backward compatibility).
 * \version According to DXF R14.
 */
int
dxf_spatial_filter_write
(
        DxfFile *fp,
                /*!< DXF file pointer to an output file (or device). */
        DxfSpatialFilter *spatial_filter
                /*!< a pointer to the DXF \c SPATIAL_FILTER object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        char *dxf_entity_name = strdup ("SPATIAL_FILTER");
        int i;

        /* Do some basic checks. */
        if (fp == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL file pointer was passed.\n")),
                  __FUNCTION__);
                /* Clean up. */
                free (dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (spatial_filter == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                /* Clean up. */
                free (dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (spatial_filter->number_of_points < 2)
        {
                fprintf (stderr,
                  (_("Error in %s () number of the clip boundary definition points is too small.\n")),
                  __FUNCTION__);
                fprintf (stderr,
                  (_("\tThe %s object with id-code %x is dropped from output.\n")),
                  dxf_entity_name, spatial_filter->id_code);
                /* Clean up. */
                free (dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (fp->acad_version_number < AutoCAD_14)
        {
                fprintf (stderr,
                  (_("Warning in %s () illegal DXF version for this %s entity with id-code: %x.\n")),
                  __FUNCTION__, dxf_entity_name, spatial_filter->id_code);
        }
        /* Start writing output. */
        fprintf (fp->fp, "  0\n%s\n", dxf_entity_name);
        if (spatial_filter->id_code != -1)
        {
                fprintf (fp->fp, "  5\n%x\n", spatial_filter->id_code);
        }
        /*!
         * \todo for version R14.\n
         * Implementing the start of application-defined group
         * "{application_name", with Group code 102.\n
         * For example: "{ACAD_REACTORS" indicates the start of the
         * AutoCAD persistent reactors group.\n\n
         * application-defined codes: Group codes and values within the
         * 102 groups are application defined (optional).\n\n
         * End of group, "}" (optional), with Group code 102.
         */
        if ((strcmp (spatial_filter->dictionary_owner_soft, "") != 0)
          && (fp->acad_version_number >= AutoCAD_14))
        {
                fprintf (fp->fp, "102\n{ACAD_REACTORS\n");
                fprintf (fp->fp, "330\n%s\n", spatial_filter->dictionary_owner_soft);
                fprintf (fp->fp, "102\n}\n");
        }
        if ((strcmp (spatial_filter->dictionary_owner_hard, "") != 0)
          && (fp->acad_version_number >= AutoCAD_14))
        {
                fprintf (fp->fp, "102\n{ACAD_XDICTIONARY\n");
                fprintf (fp->fp, "360\n%s\n", spatial_filter->dictionary_owner_hard);
                fprintf (fp->fp, "102\n}\n");
        }
        if (fp->acad_version_number >= AutoCAD_13)
        {
                fprintf (fp->fp, "100\nAcDbFilter\n");
                fprintf (fp->fp, "100\nAcDbSpatialFilter\n");
        }
        fprintf (fp->fp, " 70\n%d\n", spatial_filter->number_of_points);
        for (i = 0; i < spatial_filter->number_of_points; i++)
        {
                fprintf (fp->fp, " 10\n%f\n", spatial_filter->x0[i]);
                fprintf (fp->fp, " 20\n%f\n", spatial_filter->y0[i]);
                /*! \warning We do rely on a correct number of clip
                 * boundary definition points in the instnce of the
                 * DxfSpatialFilter struct.\n
                 * It is possible to run into a part of the array where
                 * initial values (0.0) are present. */
        }
        if ((fp->acad_version_number >= AutoCAD_12)
                && (spatial_filter->extr_x0 != 0.0)
                && (spatial_filter->extr_y0 != 0.0)
                && (spatial_filter->extr_z0 != 1.0))
        {
                fprintf (fp->fp, "210\n%f\n", spatial_filter->extr_x0);
                fprintf (fp->fp, "220\n%f\n", spatial_filter->extr_y0);
                fprintf (fp->fp, "230\n%f\n", spatial_filter->extr_z0);
        }
        fprintf (fp->fp, " 71\n%d\n", spatial_filter->clip_boundary_display_flag);
        fprintf (fp->fp, " 72\n%d\n", spatial_filter->front_clipping_plane_flag);
        if (spatial_filter->front_clipping_plane_flag)
        {
                fprintf (fp->fp, " 40\n%f\n", spatial_filter->front_clipping_plane_distance);
        }
        fprintf (fp->fp, " 73\n%d\n", spatial_filter->back_clipping_plane_flag);
        if (spatial_filter->back_clipping_plane_flag)
        {
                fprintf (fp->fp, " 41\n%f\n", spatial_filter->back_clipping_plane_distance);
        }
        for (i = 0; i < 12; i++)
        {
                fprintf (fp->fp, " 40\n%f\n", spatial_filter->inverse_block_transformation[i]);
        }
        for (i = 0; i < 12; i++)
        {
                fprintf (fp->fp, " 40\n%f\n", spatial_filter->block_transformation[i]);
        }
        /* Clean up. */
        free (dxf_entity_name);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a DXF \c SPATIAL_FILTER and all it's
 * data fields.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12 (backward compatibility).
 * \version According to DXF R13 (backward compatibility).
 * \version According to DXF R14.
 */
int
dxf_spatial_filter_free
(
        DxfSpatialFilter *spatial_filter
                /*!< a pointer to the memory occupied by the DXF
                 * \c SPATIAL_FILTER object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (spatial_filter->next != NULL)
        {
              fprintf (stderr,
                (_("Error in %s () pointer to next DxfSpatialFilter was not NULL.\n")),
                __FUNCTION__);
              return (EXIT_FAILURE);
        }
        free (spatial_filter->dictionary_owner_soft);
        free (spatial_filter->dictionary_owner_hard);
        free (spatial_filter);
        spatial_filter = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a chain of DXF \c SPATIAL_FILTER
 * objects and all their data fields.
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12 (backward compatibility).
 * \version According to DXF R13 (backward compatibility).
 * \version According to DXF R14.
 */
void
dxf_spatial_filter_free_chain
(
        DxfSpatialFilter *spatial_filters
                /*!< a pointer to the chain of DXF \c SPATIAL_FILTER objects. */
)
{
#ifdef DEBUG
        DXF_DEBUG_BEGIN
#endif
        if (spatial_filters == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
        }
        while (spatial_filters != NULL)
        {
                struct DxfSpatialFilter *iter = spatial_filters->next;
                dxf_spatial_filter_free (spatial_filters);
                spatial_filters = (DxfSpatialFilter *) iter;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
}


/* EOF*/
