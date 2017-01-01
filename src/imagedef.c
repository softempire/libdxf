/*!
 * \file imagedef.c
 *
 * \author Copyright (C) 2015 ... 2016 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 *
 * \brief Functions for a DXF imagedef object (\c IMAGEDEF).
 *
 * \since The \c IMAGEDEF object was introduced in DXF R14.
 *
 * \warning The \c IMAGEDEF object requires AutoCAD version R14 or higher.
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12 (backward compatibility).
 * \version According to DXF R13 (backward compatibility).
 * \version According to DXF R14.
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


#include "imagedef.h"


/*!
 * \brief Allocate memory for a \c DxfImagedef.
 *
 * Fill the memory contents with zeros.
 */
DxfImagedef *
dxf_imagedef_new ()
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        DxfImagedef *imagedef = NULL;
        size_t size;

        size = sizeof (DxfImagedef);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((imagedef = malloc (size)) == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfImagedef struct.\n")),
                  __FUNCTION__);
                imagedef = NULL;
        }
        else
        {
                memset (imagedef, 0, size);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (imagedef);
}


/*!
 * \brief Allocate memory and initialize data fields in a \c IMAGEDEF
 * object.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 */
DxfImagedef *
dxf_imagedef_init
(
        DxfImagedef *imagedef
                /*!< DXF \c IMAGEDEF object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        int i;

        /* Do some basic checks. */
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                imagedef = dxf_imagedef_new ();
        }
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfImagedef struct.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        imagedef->id_code = 0;
        imagedef->dictionary_owner_soft = strdup ("");
        imagedef->dictionary_owner_hard = strdup ("");
        imagedef->file_name = strdup ("");
        imagedef->x0 = 0.0;
        imagedef->y0 = 0.0;
        imagedef->x1 = 0.0;
        imagedef->y1 = 0.0;
        imagedef->class_version = 0;
        imagedef->image_is_loaded_flag = 0;
        imagedef->resolution_units = 0;
        imagedef->acad_image_dict_soft = strdup ("");
        for (i = 0; i < DXF_MAX_PARAM; i++)
        {
                imagedef->imagedef_reactor_soft[i] = strdup ("");
        }
        imagedef->next = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (imagedef);
}


/*!
 * \brief Read data from a DXF file into a DXF \c IMAGEDEF object.
 *
 * The last line read from file contained the string "IMAGEDEF". \n
 * Now follows some data for the \c IMAGEDEF, to be terminated with a
 * "  0" string announcing the following entity, or the end of the
 * \c ENTITY section marker \c ENDSEC. \n
 * While parsing the DXF file store data in \c imagedef. \n
 *
 * \return \c a pointer to \c imagedef.
 */
DxfImagedef *
dxf_imagedef_read
(
        DxfFile *fp,
                /*!< DXF file pointer to an input file (or device). */
        DxfImagedef *imagedef
                /*!< DXF \c IMAGEDEF object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        char *temp_string = NULL;
        int i;

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
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                imagedef = dxf_imagedef_new ();
                imagedef = dxf_imagedef_init (imagedef);
        }
        i = 0;
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
                else if (strcmp (temp_string, "1") == 0)
                {
                        /* Now follows a string containing a file name. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", imagedef->file_name);
                }
                if (strcmp (temp_string, "5") == 0)
                {
                        /* Now follows a string containing a sequential
                         * id number. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%x\n", &imagedef->id_code);
                }
                else if (strcmp (temp_string, "10") == 0)
                {
                        /* Now follows a string containing the
                         * U-value of the image size in pixels. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &imagedef->x0);
                }
                else if (strcmp (temp_string, "20") == 0)
                {
                        /* Now follows a string containing the
                         * V-value of the image size in pixels. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &imagedef->y0);
                }
                else if (strcmp (temp_string, "11") == 0)
                {
                        /* Now follows a string containing the
                         * U-value of the default size of one pixel in
                         * AutoCAD units. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &imagedef->x1);
                }
                else if (strcmp (temp_string, "12") == 0)
                {
                        /* Now follows a string containing the
                         * V-value of the default size of one pixel in
                         * AutoCAD units. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &imagedef->y1);
                }
                else if (strcmp (temp_string, "90") == 0)
                {
                        /* Now follows a string containing the
                         * value of class version. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &imagedef->class_version);
                }
                else if ((fp->acad_version_number >= AutoCAD_13)
                        && (strcmp (temp_string, "100") == 0))
                {
                        /* Now follows a string containing the
                         * subclass marker value. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", temp_string);
                        if (strcmp (temp_string, "AcDbRasterImageDef") != 0)
                        {
                                fprintf (stderr,
                                  (_("Warning in %s () found a bad subclass marker in: %s in line: %d.\n")),
                                  __FUNCTION__, fp->filename, fp->line_number);
                        }
                }
                else if (strcmp (temp_string, "280") == 0)
                {
                        /* Now follows a string containing the
                         * value of the image is loaded flag. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &imagedef->image_is_loaded_flag);
                }
                else if (strcmp (temp_string, "281") == 0)
                {
                        /* Now follows a string containing the
                         * value of the resolution units. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &imagedef->resolution_units);
                }
                else if ((strcmp (temp_string, "330") == 0)
                  && (i == 0))
                {
                        /* Now follows a string containing Soft-pointer
                         * ID/handle to owner dictionary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", imagedef->dictionary_owner_soft);
                        i++;
                }
                else if ((strcmp (temp_string, "330") == 0)
                  && (i == 1))
                {
                        /* Now follows a string containing Soft-pointer
                         * ID/handle to owner dictionary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", imagedef->acad_image_dict_soft);
                        i++;
                        /*! \todo Check for overrun of array index. */
                }
                else if ((strcmp (temp_string, "330") == 0)
                  && (i > 1))
                {
                        /* Now follows a string containing a Soft
                         * pointer reference to entity. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", imagedef->imagedef_reactor_soft[i]);
                        i++;
                }
                else if (strcmp (temp_string, "360") == 0)
                {
                        /* Now follows a string containing Hard owner
                         * ID/handle to owner dictionary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", imagedef->dictionary_owner_hard);
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
        return (imagedef);
}


/*!
 * \brief Write DXF output to a file for a DXF \c IMAGEDEF object.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_imagedef_write
(
        DxfFile *fp,
                /*!< DXF file pointer to an output file (or device). */
        DxfImagedef *imagedef
                /*!< DXF \c IMAGEDEF object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        char *dxf_entity_name = strdup ("IMAGEDEF");
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
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                /* Clean up. */
                free (dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (fp->acad_version_number < AutoCAD_14)
        {
                fprintf (stderr,
                  (_("Warning in %s () illegal DXF version for this %s entity with id-code: %x.\n")),
                  __FUNCTION__, dxf_entity_name, imagedef->id_code);
        }
        /* Start writing output. */
        fprintf (fp->fp, "  0\n%s\n", dxf_entity_name);
        if (imagedef->id_code != -1)
        {
                fprintf (fp->fp, "  5\n%x\n", imagedef->id_code);
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
        if ((strcmp (imagedef->dictionary_owner_soft, "") != 0)
          && (fp->acad_version_number >= AutoCAD_14))
        {
                fprintf (fp->fp, "102\n{ACAD_REACTORS\n");
                fprintf (fp->fp, "330\n%s\n", imagedef->dictionary_owner_soft);
                fprintf (fp->fp, "330\n%s\n", imagedef->acad_image_dict_soft);
                i = 2;
                while (strlen (imagedef->imagedef_reactor_soft[i]) > 0)
                {
                        fprintf (fp->fp, "330\n%s\n", imagedef->imagedef_reactor_soft[i]);
                        i++;
                        /*! \todo Check for overrun of array index. */
                }
                fprintf (fp->fp, "102\n}\n");
        }
        if ((strcmp (imagedef->dictionary_owner_hard, "") != 0)
          && (fp->acad_version_number >= AutoCAD_14))
        {
                fprintf (fp->fp, "102\n{ACAD_XDICTIONARY\n");
                fprintf (fp->fp, "360\n%s\n", imagedef->dictionary_owner_hard);
                fprintf (fp->fp, "102\n}\n");
        }
        if (fp->acad_version_number >= AutoCAD_13)
        {
                fprintf (fp->fp, "100\nAcDbRasterImageDef\n");
        }
        fprintf (fp->fp, " 90\n%d\n", imagedef->class_version);
        fprintf (fp->fp, "  1\n%s\n", imagedef->file_name);
        fprintf (fp->fp, " 10\n%f\n", imagedef->x0);
        fprintf (fp->fp, " 20\n%f\n", imagedef->y0);
        fprintf (fp->fp, " 11\n%f\n", imagedef->x1);
        fprintf (fp->fp, " 12\n%f\n", imagedef->y1);
        fprintf (fp->fp, "280\n%d\n", imagedef->image_is_loaded_flag);
        fprintf (fp->fp, "281\n%d\n", imagedef->resolution_units);
        /* Clean up. */
        free (dxf_entity_name);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a DXF \c IMAGEDEF and all it's
 * data fields.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_imagedef_free
(
        DxfImagedef *imagedef
                /*!< Pointer to the memory occupied by the DXF
                 * \c IMAGEDEF object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        int i;

        /* Do some basic checks. */
        if (imagedef->next != NULL)
        {
              fprintf (stderr,
                (_("Error in %s () pointer to next DxfImagedef was not NULL.\n")),
                __FUNCTION__);
              return (EXIT_FAILURE);
        }
        free (imagedef->dictionary_owner_soft);
        free (imagedef->dictionary_owner_hard);
        free (imagedef->file_name);
        free (imagedef->acad_image_dict_soft);
        for (i = 0; i < DXF_MAX_PARAM; i++)
        {
                free (imagedef->imagedef_reactor_soft[i]);
        }
        free (imagedef);
        imagedef = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a chain of DXF \c IMAGEDEF
 * objects and all their data fields.
 */
void
dxf_imagedef_free_chain
(
        DxfImagedef *imagedefs
                /*!< pointer to the chain of DXF \c IMAGEDEF objects. */
)
{
#ifdef DEBUG
        DXF_DEBUG_BEGIN
#endif
        if (imagedefs == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
        }
        while (imagedefs != NULL)
        {
                struct DxfImagedef *iter = imagedefs->next;
                dxf_imagedef_free (imagedefs);
                imagedefs = (DxfImagedef *) iter;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
}


/*!
 * \brief Get the \c id_code from a DXF \c IMAGEDEF object.
 *
 * \return \c id_code when successful, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_imagedef_get_id_code
(
        DxfImagedef *imagedef
                /*!< a pointer to a DXF \c IMAGEDEF object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (imagedef->id_code < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was found in the id_code member.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (imagedef->id_code);
}


/*!
 * \brief Set the \c id_code for a DXF \c IMAGEDEF object.
 *
 * \return a pointer to \c imagedef when successful, or \c NULL when an
 * error occurred.
 */
DxfImagedef *
dxf_imagedef_set_id_code
(
        DxfImagedef *imagedef,
                /*!< a pointer to a DXF \c IMAGEDEF object. */
        int id_code
                /*!< Identification number for the entity.\n
                 * This is to be an unique (sequential) number in the DXF
                 * file. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (id_code < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative id-code value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        imagedef->id_code = id_code;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (imagedef);
}


/*!
 * \brief Get the pointer to the \c dictionary_owner_soft from a DXF
 * \c IMAGEDEF object.
 *
 * \return a pointer to \c dictionary_owner_soft when successful, or
 * \c NULL when an error occurred.
 *
 * \warning No checks are performed on the returned pointer.
 */
char *
dxf_imagedef_get_dictionary_owner_soft
(
        DxfImagedef *imagedef
                /*!< a pointer to a DXF \c IMAGEDEF object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (imagedef->dictionary_owner_soft ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the dictionary_owner_soft member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (imagedef->dictionary_owner_soft));
}


/*!
 * \brief Set the pointer to the \c dictionary_owner_soft for a DXF
 * \c IMAGEDEF object.
 *
 * \return a pointer to \c imagedef when successful, or \c NULL when an
 * error occurred.
 */
DxfImagedef *
dxf_imagedef_set_dictionary_owner_soft
(
        DxfImagedef *imagedef,
                /*!< a pointer to a DXF \c IMAGEDEF object. */
        char *dictionary_owner_soft
                /*!< a string containing the pointer to the \c
                 * dictionary_owner_soft for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (dictionary_owner_soft == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        imagedef->dictionary_owner_soft = strdup (dictionary_owner_soft);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (imagedef);
}


/*!
 * \brief Get the pointer to the \c dictionary_owner_hard from a DXF
 * \c IMAGEDEF object.
 *
 * \return a pointer to \c dictionary_owner_hard when successful, or
 * \c NULL when an error occurred.
 *
 * \warning No checks are performed on the returned pointer.
 */
char *
dxf_imagedef_get_dictionary_owner_hard
(
        DxfImagedef *imagedef
                /*!< a pointer to a DXF \c IMAGEDEF object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (imagedef->dictionary_owner_hard ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the dictionary_owner_hard member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (imagedef->dictionary_owner_hard));
}


/*!
 * \brief Set the pointer to the \c dictionary_owner_hard for a DXF
 * \c IMAGEDEF object.
 *
 * \return a pointer to \c imagedef when successful, or \c NULL when an
 * error occurred.
 */
DxfImagedef *
dxf_imagedef_set_dictionary_owner_hard
(
        DxfImagedef *imagedef,
                /*!< a pointer to a DXF \c IMAGEDEF object. */
        char *dictionary_owner_hard
                /*!< a string containing the pointer to the \c
                 * dictionary_owner_hard for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (dictionary_owner_hard == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        imagedef->dictionary_owner_hard = strdup (dictionary_owner_hard);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (imagedef);
}


/*!
 * \brief Get the \c file_name from a DXF \c IMAGEDEF object.
 *
 * \return \c file_name when sucessful, or \c NULL when an error occurred.
 */
char *
dxf_imagedef_get_file_name
(
        DxfImagedef *imagedef
                /*!< a pointer to a DXF \c IMAGEDEF object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (imagedef->file_name ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the file_name member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (imagedef->file_name));
}


/*!
 * \brief Set the \c file_name for a DXF \c IMAGEDEF object.
 *
 * \return a pointer to \c imagedef when successful, or \c NULL when an
 * error occurred.
 */
DxfImagedef *
dxf_imagedef_set_file_name
(
        DxfImagedef *imagedef,
                /*!< a pointer to a DXF \c IMAGEDEF object. */
        char *file_name
                /*!< a string containing the \c file_name for the
                 * object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (file_name == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        imagedef->file_name = strdup (file_name);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (imagedef);
}


/*!
 * \brief Get the image size in pixels \c p0 of a DXF \c IMAGEDEF object.
 *
 * \return the image size in pixels \c p0.
 */
DxfPoint *
dxf_imagedef_get_p0
(
        DxfImagedef *imagedef
                /*!< a pointer to a DXF \c IMAGEDEF object. */
)
{
#ifdef DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (imagedef->p0 == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (imagedef->p0);
}


/*!
 * \brief Set the image size in pixels \c p0 of a DXF \c IMAGEDEF object.
 *
 * \return a pointer to a DXF \c IMAGEDEF object.
 */
DxfImagedef *
dxf_imagedef_set_p0
(
        DxfImagedef *imagedef,
                /*!< a pointer to a DXF \c IMAGEDEF object. */
        DxfPoint *p0
                /*!< a pointer to a DXF \c POINT entity. */
)
{
#ifdef DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (imagedef == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (p0 == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        imagedef->p0 = p0;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (imagedef);
}


/* EOF*/
