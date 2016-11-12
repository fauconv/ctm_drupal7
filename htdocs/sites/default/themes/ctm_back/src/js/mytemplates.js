/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
// Register a template definition set named "default".
CKEDITOR.addTemplates( 'default',
{
    // The name of the subfolder that contains the preview images of the templates.
    imagesPath: CKEDITOR.getUrl(Drupal.settings.ckeditor.base_ctm_back + '/img/templates/'),

    // Template definitions.
    templates:
      [
        {
          title: '2 columns [1-1]',
          image: 'template-bloc-1-1.gif',
          description: '2 columns template [ 50%-50% ]',
          html:
            '<div class="columns" contenteditable="false">' +
              '<div style="width:50%" contenteditable="true">' +
                '<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin tristique lacus vitae magna tincidunt, at adipiscing erat ornare.</p>' +
              '</div>' +
              '<div style="width:50%" contenteditable="true">' +
                '<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin tristique lacus vitae magna tincidunt, at adipiscing erat ornare.</p>' +
              '</div>' +
            '</div><br />'
        }
      ]
});

