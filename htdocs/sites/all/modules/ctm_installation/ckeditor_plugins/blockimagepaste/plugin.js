
(function(){CKEDITOR.plugins.add("blockimagepaste",{init:function(a){function c(e){var d=e.replace(/<img[^>]*src="data:image\/(bmp|dds|gif|jpg|jpeg|png|psd|pspimage|tga|thm|tif|tiff|yuv|ai|eps|ps|svg);base64,.*?"[^>]*>/gi,function(f){return""});return d}function b(){if(a.readOnly){return}setTimeout(function(){a.document.$.body.innerHTML=c(a.document.$.body.innerHTML)},100)}a.on("contentDom",function(){a.document.on("drop",b);a.document.getBody().on("drop",b)});a.on("paste",function(f){var d=f.data.dataValue;if(!d){return}f.data.dataValue=c(d)})}})})();