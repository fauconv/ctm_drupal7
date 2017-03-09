Field from theme
by Alexandre Delage and Christian Vallebella (fauconv5)

Overview:
---------

Usage :
------

1. activate the module as usual

2. add a blob field in a content type and select "Data from theme" as widget

3. Select "Data from theme" as field formater

4. Give permission to use template picker with your content type

5. create a template with name node--[content type]--tp--[a name].tpl.php
   in your theme.

6. in the template use the function ttf('#my tag#',$content['[my_field_name]'])
to retrive the text corresponding to "my tag" from the field my_field_name

7. create a node corresponding to your content type and select the template
with template picker. Save it. Then you can re-edit to see all your tag.

(TODO : add ajax call to refresh data from theme widget)
