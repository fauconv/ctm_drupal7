/*
Copyright (c) 2003-2013, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/

/*
 * This file is used/requested by the 'Styles' button.
 * The 'Styles' button is not enabled by default in DrupalFull and DrupalFiltered toolbars.
 */
if(typeof(CKEDITOR) !== 'undefined') {
    CKEDITOR.addStylesSet( 'drupal',
    [

            { name : 'Cadre', element : 'div', attributes : {'class':'cadre'}},
            { name : 'bordure', element : 'div', attributes : {'class':'bordure'}},

            { name : 'PDF', element : 'span', attributes : {'class':'pdf16'}},
            { name : 'Word', element : 'span', attributes : {'class':'word16'}},
            { name : 'Excel', element : 'span', attributes : {'class':'excel16'}},
            { name : 'Powerpoint', element : 'span', attributes : {'class':'powerpoint16'}},
            { name : 'Image', element : 'span', attributes : {'class':'image16'}},

    ]);
}
