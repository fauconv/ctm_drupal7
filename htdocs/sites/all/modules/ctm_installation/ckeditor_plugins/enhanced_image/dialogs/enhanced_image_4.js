!function(){var t=function(t,e){function i(){var t=arguments,e=this.getContentElement("advanced","txtdlgGenStyle");e&&e.commit.apply(e,t),this.foreach(function(e){e.commit&&"txtdlgGenStyle"!=e.id&&e.commit.apply(e,t)})}function a(t){if(!n){n=1;var e=this.getDialog(),i=e.imageElement;if(i){this.commit(s,i),t=[].concat(t);for(var a,l=t.length,o=0;l>o;o++)a=e.getContentElement.apply(e,t[o].split(":")),a&&a.setup(s,i)}n=0}}var n,l,s=1,o=2,r=4,m=8,g=/^\s*(\d+)((px)|\%)?\s*$/i,d=/(^\s*(\d+)((px)|\%)?\s*$)|^$/i,h=/^\d+px$/,u=function(){var t=this.getValue(),e=this.getDialog(),i=t.match(g);if(i&&("%"==i[2]&&p(e,!1),t=i[1]),e.lockRatio){var a=e.originalElement;"true"==a.getCustomData("isReady")&&("txtHeight"==this.id?(t&&"0"!=t&&(t=Math.round(a.$.width*(t/a.$.height))),isNaN(t)||e.setValueOf("info","txtWidth",t)):(t&&"0"!=t&&(t=Math.round(a.$.height*(t/a.$.width))),isNaN(t)||e.setValueOf("info","txtHeight",t)))}c(e)},c=function(t){return t.originalElement&&t.preview?(t.commitContent(r,t.preview),0):1},p=function(t,e){if(!t.getContentElement("info","ratioLock"))return null;var i=t.originalElement;if(!i)return null;if("check"==e){if(!t.userlockRatio&&"true"==i.getCustomData("isReady")){var a=t.getValueOf("info","txtWidth"),n=t.getValueOf("info","txtHeight"),l=1e3*i.$.width/i.$.height,s=1e3*a/n;t.lockRatio=!1,a||n?isNaN(l)||isNaN(s)||Math.round(l)==Math.round(s)&&(t.lockRatio=!0):t.lockRatio=!0}}else void 0!=e?t.lockRatio=e:(t.userlockRatio=1,t.lockRatio=!t.lockRatio);var o=CKEDITOR.document.getById(C);if(t.lockRatio?o.removeClass("cke_btn_unlocked"):o.addClass("cke_btn_unlocked"),o.setAttribute("aria-checked",t.lockRatio),CKEDITOR.env.hc){var r=o.getChild(0);r.setHtml(t.lockRatio?CKEDITOR.env.ie?"â– ":"â–£":CKEDITOR.env.ie?"â–¡":"â–¢")}return t.lockRatio},v=function(t){var e=t.originalElement;if("true"==e.getCustomData("isReady")){var i=t.getContentElement("info","txtWidth"),a=t.getContentElement("info","txtHeight");i&&i.setValue(e.$.width),a&&a.setValue(e.$.height)}c(t)},f=function(t,e){function i(t,e){var i=t.match(g);return i?("%"==i[2]&&(i[1]+="%",p(a,!1)),i[1]):e}if(t==s){var a=this.getDialog(),n="",l="txtWidth"==this.id?"width":"height",o=e.getAttribute(l);o&&(n=i(o,n)),n=i(e.getStyle(l),n),this.setValue(n)}},y=function(){var t=this.originalElement;t.setCustomData("isReady","true"),t.removeListener("load",y),t.removeListener("error",b),t.removeListener("abort",b),CKEDITOR.document.getById(x).setStyle("display","none"),this.dontResetSize||v(this),this.firstLoad&&CKEDITOR.tools.setTimeout(function(){p(this,"check")},0,this),this.firstLoad=!1,this.dontResetSize=!1},b=function(){var t=this.originalElement;t.removeListener("load",y),t.removeListener("error",b),t.removeListener("abort",b);var e=CKEDITOR.getUrl(CKEDITOR.plugins.get("enhanced_image").path+"images/noimage.png");this.preview&&this.preview.setAttribute("src",e),CKEDITOR.document.getById(x).setStyle("display","none"),p(this,!1)},E=function(t){return CKEDITOR.tools.getNextId()+"_"+t},C=E("btnLockSizes"),S=E("btnResetSize"),x=E("ImagePreviewLoader"),k=E("previewLink"),w=E("previewImage");return{title:t.lang.image["image"==e?"title":"titleButton"],minWidth:420,minHeight:360,onShow:function(){this.imageElement=!1,this.linkElement=!1,this.imageEditMode=!1,this.linkEditMode=!1,this.lockRatio=!0,this.userlockRatio=0,this.dontResetSize=!1,this.firstLoad=!0,this.addLink=!1;var t=this.getParentEditor(),i=t.getSelection(),a=i&&i.getSelectedElement(),n=a&&t.elementPath(a).contains("a",1);if(CKEDITOR.document.getById(x).setStyle("display","none"),l=new CKEDITOR.dom.element("img",t.document),this.preview=CKEDITOR.document.getById(w),this.originalElement=t.document.createElement("img"),this.originalElement.setAttribute("alt",""),this.originalElement.setCustomData("isReady","false"),n){this.linkElement=n,this.linkEditMode=!0;var r=n.getChildren();if(1==r.count()){var m=r.getItem(0).getName();("img"==m||"input"==m)&&(this.imageElement=r.getItem(0),"img"==this.imageElement.getName()?this.imageEditMode="img":"input"==this.imageElement.getName()&&(this.imageEditMode="input"))}"image"==e&&this.setupContent(o,n)}(a&&"img"==a.getName()&&!a.data("cke-realelement")||a&&"input"==a.getName()&&"image"==a.getAttribute("type"))&&(this.imageEditMode=a.getName(),this.imageElement=a),this.imageEditMode?(this.cleanImageElement=this.imageElement,this.imageElement=this.cleanImageElement.clone(!0,!0),this.setupContent(s,this.imageElement)):this.imageElement=t.document.createElement("img"),p(this,!0),CKEDITOR.tools.trim(this.getValueOf("info","txtUrl"))||(this.preview.removeAttribute("src"),this.preview.setStyle("display","none"))},onOk:function(){if(this.imageEditMode){var i=this.imageEditMode;"image"==e&&"input"==i&&confirm(t.lang.image.button2Img)?(i="img",this.imageElement=t.document.createElement("img"),this.imageElement.setAttribute("alt",""),t.insertElement(this.imageElement)):"image"!=e&&"img"==i&&confirm(t.lang.image.img2Button)?(i="input",this.imageElement=t.document.createElement("input"),this.imageElement.setAttributes({type:"image",alt:""}),t.insertElement(this.imageElement)):(this.imageElement=this.cleanImageElement,delete this.cleanImageElement)}else"image"==e?this.imageElement=t.document.createElement("img"):(this.imageElement=t.document.createElement("input"),this.imageElement.setAttribute("type","image")),this.imageElement.setAttribute("alt","");this.linkEditMode||(this.linkElement=t.document.createElement("a")),this.commitContent(s,this.imageElement),this.commitContent(o,this.linkElement),this.imageElement.getAttribute("style")||this.imageElement.removeAttribute("style"),this.imageEditMode?!this.linkEditMode&&this.addLink?(t.insertElement(this.linkElement),this.imageElement.appendTo(this.linkElement)):this.linkEditMode&&!this.addLink&&(t.getSelection().selectElement(this.linkElement),t.insertElement(this.imageElement)):this.addLink?this.linkEditMode?t.insertElement(this.imageElement):(t.insertElement(this.linkElement),this.linkElement.append(this.imageElement,!1)):t.insertElement(this.imageElement)},onLoad:function(){"image"!=e&&this.hidePage("Link");var t=this._.element.getDocument();this.getContentElement("info","ratioLock")&&(this.addFocusable(t.getById(S),5),this.addFocusable(t.getById(C),5)),this.commitContent=i},onHide:function(){this.preview&&this.commitContent(m,this.preview),this.originalElement&&(this.originalElement.removeListener("load",y),this.originalElement.removeListener("error",b),this.originalElement.removeListener("abort",b),this.originalElement.remove(),this.originalElement=!1),delete this.imageElement},contents:[{id:"info",label:t.lang.image.infoTab,accessKey:"I",elements:[{type:"vbox",padding:0,children:[{type:"hbox",widths:["280px","110px"],align:"right",children:[{id:"txtUrl",type:"text",label:t.lang.common.url,required:!0,onChange:function(){var t=this.getDialog(),e=this.getValue();if(e.length>0){t=this.getDialog();var i=t.originalElement;t.preview.removeStyle("display"),i.setCustomData("isReady","false");var a=CKEDITOR.document.getById(x);a&&a.setStyle("display",""),i.on("load",y,t),i.on("error",b,t),i.on("abort",b,t),i.setAttribute("src",e),l.setAttribute("src",e),t.preview.setAttribute("src",l.$.src),c(t)}else t.preview&&(t.preview.removeAttribute("src"),t.preview.setStyle("display","none"))},setup:function(t,e){if(t==s){var i=e.data("cke-saved-src")||e.getAttribute("src"),a=this;this.getDialog().dontResetSize=!0,a.setValue(i),a.setInitValue()}},commit:function(t,e){t==s&&(this.getValue()||this.isChanged())?(e.data("cke-saved-src",this.getValue()),e.setAttribute("src",this.getValue())):t==m&&(e.setAttribute("src",""),e.removeAttribute("src"))},validate:CKEDITOR.dialog.validate.notEmpty(t.lang.image.urlMissing)},{type:"button",id:"browse",style:"display:inline-block;margin-top:10px;",align:"center",label:t.lang.common.browseServer,hidden:!0,filebrowser:"info:txtUrl"}]}]},{id:"txtAlt",type:"text",label:t.lang.image.alt,accessKey:"T","default":"",onChange:function(){c(this.getDialog())},setup:function(t,e){t==s&&this.setValue(e.getAttribute("alt"))},validate:CKEDITOR.dialog.validate.notEmpty("Le texte de remplacement est vide"),commit:function(t,e){t==s?(this.getValue()||this.isChanged())&&(e.setAttribute("alt",this.getValue()),e.setAttribute("title",this.getValue())):t==r?(e.setAttribute("alt",this.getValue()),e.setAttribute("title",this.getValue())):t==m&&(e.removeAttribute("alt"),e.removeAttribute("title"))}},{type:"hbox",children:[{id:"basic",type:"vbox",children:[{type:"hbox",requiredContent:"img{width,height}",widths:["50%","50%"],children:[{type:"vbox",padding:1,children:[{type:"text",width:"45px",id:"txtWidth",label:t.lang.common.width,onKeyUp:u,onChange:function(){a.call(this,"advanced:txtdlgGenStyle")},validate:function(){var e=this.getValue().match(d),i=!(!e||0===parseInt(e[1],10));return i||alert(t.lang.common.invalidWidth),i},setup:f,commit:function(t,e,i){var a=this.getValue();if(t==s)a?e.setStyle("width",CKEDITOR.tools.cssLength(a)):e.removeStyle("width"),!i&&e.removeAttribute("width");else if(t==r){var n=a.match(g);if(n)e.setStyle("width",CKEDITOR.tools.cssLength(a));else{var l=this.getDialog().originalElement;"true"==l.getCustomData("isReady")&&e.setStyle("width",l.$.width+"px")}}else t==m&&(e.removeAttribute("width"),e.removeStyle("width"))}},{type:"text",id:"txtHeight",width:"45px",label:t.lang.common.height,onKeyUp:u,onChange:function(){a.call(this,"advanced:txtdlgGenStyle")},validate:function(){var e=this.getValue().match(d),i=!(!e||0===parseInt(e[1],10));return i||alert(t.lang.common.invalidHeight),i},setup:f,commit:function(t,e,i){var a=this.getValue();if(t==s)a?e.setStyle("height",CKEDITOR.tools.cssLength(a)):e.removeStyle("height"),!i&&e.removeAttribute("height");else if(t==r){var n=a.match(g);if(n)e.setStyle("height",CKEDITOR.tools.cssLength(a));else{var l=this.getDialog().originalElement;"true"==l.getCustomData("isReady")&&e.setStyle("height",l.$.height+"px")}}else t==m&&(e.removeAttribute("height"),e.removeStyle("height"))}}]},{id:"ratioLock",type:"html",style:"margin-top:30px;width:40px;height:40px;",onLoad:function(){var t=CKEDITOR.document.getById(S),e=CKEDITOR.document.getById(C);t&&(t.on("click",function(t){v(this),t.data&&t.data.preventDefault()},this.getDialog()),t.on("mouseover",function(){this.addClass("cke_btn_over")},t),t.on("mouseout",function(){this.removeClass("cke_btn_over")},t)),e&&(e.on("click",function(t){var e=(p(this),this.originalElement),i=this.getValueOf("info","txtWidth");if("true"==e.getCustomData("isReady")&&i){var a=e.$.height/e.$.width*i;isNaN(a)||(this.setValueOf("info","txtHeight",Math.round(a)),c(this))}t.data&&t.data.preventDefault()},this.getDialog()),e.on("mouseover",function(){this.addClass("cke_btn_over")},e),e.on("mouseout",function(){this.removeClass("cke_btn_over")},e))},html:'<div><a href="javascript:void(0)" tabindex="-1" title="'+t.lang.image.lockRatio+'" class="cke_btn_locked" id="'+C+'" role="checkbox"><span class="cke_icon"></span><span class="cke_label">'+t.lang.image.lockRatio+'</span></a><a href="javascript:void(0)" tabindex="-1" title="'+t.lang.image.resetSize+'" class="cke_btn_reset" id="'+S+'" role="button"><span class="cke_label">'+t.lang.image.resetSize+"</span></a></div>"}]},{type:"vbox",padding:1,children:[{type:"text",id:"txtBorder",requiredContent:"img{border-width}",width:"60px",label:t.lang.image.border,"default":"",onKeyUp:function(){c(this.getDialog())},onChange:function(){a.call(this,"advanced:txtdlgGenStyle")},validate:CKEDITOR.dialog.validate.integer(t.lang.image.validateBorder),setup:function(t,e){if(t==s){var i,a=e.getStyle("border-width");a=a&&a.match(/^(\d+px)(?: \1 \1 \1)?$/),i=a&&parseInt(a[1],10),isNaN(parseInt(i,10))&&(i=e.getAttribute("border")),this.setValue(i)}},commit:function(t,e,i){var a=parseInt(this.getValue(),10);t==s||t==r?(isNaN(a)?!a&&this.isChanged()&&e.removeStyle("border"):(e.setStyle("border-width",CKEDITOR.tools.cssLength(a)),e.setStyle("border-style","solid")),i||t!=s||e.removeAttribute("border")):t==m&&(e.removeAttribute("border"),e.removeStyle("border-width"),e.removeStyle("border-style"),e.removeStyle("border-color"))}},{type:"text",id:"txtHSpace",requiredContent:"img{margin-left,margin-right}",width:"60px",label:t.lang.image.hSpace,"default":"",onKeyUp:function(){c(this.getDialog())},onChange:function(){a.call(this,"advanced:txtdlgGenStyle")},validate:CKEDITOR.dialog.validate.integer(t.lang.image.validateHSpace),setup:function(t,e){if(t==s){var i,a,n,l=e.getStyle("margin-left"),o=e.getStyle("margin-right");l=l&&l.match(h),o=o&&o.match(h),a=parseInt(l,10),n=parseInt(o,10),i=a==n&&a,isNaN(parseInt(i,10))&&(i=e.getAttribute("hspace")),this.setValue(i)}},commit:function(t,e,i){var a=parseInt(this.getValue(),10);t==s||t==r?(isNaN(a)?!a&&this.isChanged()&&(e.removeStyle("margin-left"),e.removeStyle("margin-right")):(e.setStyle("margin-left",CKEDITOR.tools.cssLength(a)),e.setStyle("margin-right",CKEDITOR.tools.cssLength(a))),i||t!=s||e.removeAttribute("hspace")):t==m&&(e.removeAttribute("hspace"),e.removeStyle("margin-left"),e.removeStyle("margin-right"))}},{type:"text",id:"txtVSpace",requiredContent:"img{margin-top,margin-bottom}",width:"60px",label:t.lang.image.vSpace,"default":"",onKeyUp:function(){c(this.getDialog())},onChange:function(){a.call(this,"advanced:txtdlgGenStyle")},validate:CKEDITOR.dialog.validate.integer(t.lang.image.validateVSpace),setup:function(t,e){if(t==s){var i,a,n,l=e.getStyle("margin-top"),o=e.getStyle("margin-bottom");l=l&&l.match(h),o=o&&o.match(h),a=parseInt(l,10),n=parseInt(o,10),i=a==n&&a,isNaN(parseInt(i,10))&&(i=e.getAttribute("vspace")),this.setValue(i)}},commit:function(t,e,i){var a=parseInt(this.getValue(),10);t==s||t==r?(isNaN(a)?!a&&this.isChanged()&&(e.removeStyle("margin-top"),e.removeStyle("margin-bottom")):(e.setStyle("margin-top",CKEDITOR.tools.cssLength(a)),e.setStyle("margin-bottom",CKEDITOR.tools.cssLength(a))),i||t!=s||e.removeAttribute("vspace")):t==m&&(e.removeAttribute("vspace"),e.removeStyle("margin-top"),e.removeStyle("margin-bottom"))}},{id:"cmbAlign",requiredContent:"img{float,display,margin-left,margin-right}",type:"select",widths:["35%","65%"],style:"width:90px",label:t.lang.common.align,"default":"",items:[[t.lang.common.notSet,""],[t.lang.common.alignLeft,"left"],[t.lang.common.alignCenter,"center"],[t.lang.common.alignRight,"right"]],onChange:function(){c(this.getDialog()),a.call(this,"advanced:txtdlgGenStyle")},setup:function(t,e){if(t==s){var i,a,n,l=e.getStyle("float");switch(l){case"inherit":case"none":l=""}!l&&(l=(e.getAttribute("align")||"").toLowerCase()),l||(i=e.getStyle("display"),a=e.getStyle("margin-left"),n=e.getStyle("margin-right"),"block"==i&&"auto"==a&&"auto"==n&&(l="center")),this.setValue(l)}},commit:function(t,e,i){var a=this.getValue();if(t==s||t==r){if(a)switch(a){case"left":case"right":default:e.setStyle("float",a),e.removeStyle("display");break;case"center":e.removeStyle("float"),e.setStyle("display","block"),e.setStyle("margin-left","auto"),e.setStyle("margin-right","auto")}else e.removeStyle("float"),e.removeStyle("display"),e.removeStyle("margin-right"),e.removeStyle("margin-left");if(!i&&t==s){if(a=(e.getAttribute("align")||"").toLowerCase(),!a){var n=e.getStyle("display"),l=e.getStyle("margin-left"),o=e.getStyle("margin-right");"block"==n&&"auto"==l&&"auto"==o&&(a="center")}switch(a){case"left":case"right":case"center":break;default:e.removeAttribute("align")}}}else t==m&&(e.removeStyle("float"),e.removeStyle("display"),e.removeStyle("margin-right"),e.removeStyle("margin-left"))}}]}]},{type:"vbox",height:"250px",children:[{type:"html",id:"htmlPreview",style:"width:95%;",html:"<div>"+CKEDITOR.tools.htmlEncode(t.lang.common.preview)+'<br><div id="'+x+'" class="ImagePreviewLoader" style="display:none"><div class="loading">&nbsp;</div></div><div class="ImagePreviewBox"><table><tr><td><a href="javascript:void(0)" target="_blank" onclick="return false;" id="'+k+'"><img id="'+w+'" alt="" /></a>'+(t.config.image_previewText||"Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Maecenas feugiat consequat diam. Maecenas metus. Vivamus diam purus, cursus a, commodo non, facilisis vitae, nulla. Aenean dictum lacinia tortor. Nunc iaculis, nibh non iaculis aliquam, orci felis euismod neque, sed ornare massa mauris sed velit. Nulla pretium mi et risus. Fusce mi pede, tempor id, cursus ac, ullamcorper nec, enim. Sed tortor. Curabitur molestie. Duis velit augue, condimentum at, ultrices a, luctus ut, orci. Donec pellentesque egestas eros. Integer cursus, augue in cursus faucibus, eros pede bibendum sem, in tempus tellus justo quis ligula. Etiam eget tortor. Vestibulum rutrum, est ut placerat elementum, lectus nisl aliquam velit, tempor aliquam eros nunc nonummy metus. In eros metus, gravida a, gravida sed, lobortis id, turpis. Ut ultrices, ipsum at venenatis fringilla, sem nulla lacinia tellus, eget aliquet turpis mauris non enim. Nam turpis. Suspendisse lacinia. Curabitur ac tortor ut ipsum egestas elementum. Nunc imperdiet gravida mauris.")+"</td></tr></table></div></div>"}]}]}]},{id:"Link",requiredContent:"a[href]",label:t.lang.image.linkTab,padding:0,elements:[{id:"txtUrl",type:"text",label:t.lang.common.url,style:"width: 100%","default":"",setup:function(t,e){if(t==o){var i=e.data("cke-saved-href");i||(i=e.getAttribute("href")),this.setValue(i)}},commit:function(e,i){if(e==o&&(this.getValue()||this.isChanged())){var a=decodeURI(this.getValue());i.data("cke-saved-href",a),i.setAttribute("href",a),(this.getValue()||!t.config.image_removeLinkByEmptyURL)&&(this.getDialog().addLink=!0)}}},{type:"button",id:"browse",filebrowser:{action:"Browse",target:"Link:txtUrl",url:t.config.filebrowserImageBrowseLinkUrl},style:"float:right",hidden:!0,label:t.lang.common.browseServer},{id:"cmbTarget",type:"select",requiredContent:"a[target]",label:t.lang.common.target,"default":"",items:[[t.lang.common.notSet,""],[t.lang.common.targetNew,"_blank"],[t.lang.common.targetTop,"_top"],[t.lang.common.targetSelf,"_self"],[t.lang.common.targetParent,"_parent"]],setup:function(t,e){t==o&&this.setValue(e.getAttribute("target")||"")},commit:function(t,e){t==o&&(this.getValue()||this.isChanged())&&e.setAttribute("target",this.getValue())}}]},{id:"Upload",hidden:!0,filebrowser:"uploadButton",label:t.lang.image.upload,elements:[{type:"file",id:"upload",label:t.lang.image.btnUpload,style:"height:40px",size:38},{type:"fileButton",id:"uploadButton",filebrowser:"info:txtUrl",label:t.lang.image.btnUpload,"for":["Upload","upload"]}]},{id:"advanced",label:t.lang.common.advancedTab,elements:[{type:"hbox",widths:["50%","25%","25%"],children:[{type:"text",id:"linkId",requiredContent:"img[id]",label:t.lang.common.id,setup:function(t,e){t==s&&this.setValue(e.getAttribute("id"))},commit:function(t,e){t==s&&(this.getValue()||this.isChanged())&&e.setAttribute("id",this.getValue())}},{id:"cmbLangDir",type:"select",requiredContent:"img[dir]",style:"width : 100px;",label:t.lang.common.langDir,"default":"",items:[[t.lang.common.notSet,""],[t.lang.common.langDirLtr,"ltr"],[t.lang.common.langDirRtl,"rtl"]],setup:function(t,e){t==s&&this.setValue(e.getAttribute("dir"))},commit:function(t,e){t==s&&(this.getValue()||this.isChanged())&&e.setAttribute("dir",this.getValue())}},{type:"text",id:"txtLangCode",requiredContent:"img[lang]",label:t.lang.common.langCode,"default":"",setup:function(t,e){t==s&&this.setValue(e.getAttribute("lang"))},commit:function(t,e){t==s&&(this.getValue()||this.isChanged())&&e.setAttribute("lang",this.getValue())}}]},{type:"text",id:"txtGenLongDescr",requiredContent:"img[longdesc]",label:t.lang.common.longDescr,setup:function(t,e){t==s&&this.setValue(e.getAttribute("longDesc"))},commit:function(t,e){t==s&&(this.getValue()||this.isChanged())&&e.setAttribute("longDesc",this.getValue())}},{type:"hbox",widths:["50%","50%"],children:[{type:"text",id:"txtGenClass",requiredContent:"img(cke-xyz)",label:t.lang.common.cssClass,"default":"",setup:function(t,e){t==s&&this.setValue(e.getAttribute("class"))},commit:function(t,e){t==s&&(this.getValue()||this.isChanged())&&e.setAttribute("class",this.getValue())}}]},{type:"text",id:"txtdlgGenStyle",requiredContent:"img{cke-xyz}",label:t.lang.common.cssStyle,validate:CKEDITOR.dialog.validate.inlineStyle(t.lang.common.invalidInlineStyle),"default":"",setup:function(t,e){if(t==s){var i=e.getAttribute("style");!i&&e.$.style.cssText&&(i=e.$.style.cssText),this.setValue(i);var a=e.$.style.height,n=e.$.style.width,l=(a?a:"").match(g),o=(n?n:"").match(g);this.attributesInStyle={height:!!l,width:!!o}}},onChange:function(){a.call(this,["info:cmbFloat","info:cmbAlign","info:txtVSpace","info:txtHSpace","info:txtBorder","info:txtWidth","info:txtHeight"]),c(this)},commit:function(t,e){t==s&&(this.getValue()||this.isChanged())&&e.setAttribute("style",this.getValue())}}]}]}};CKEDITOR.dialog.add("enhanced_image",function(e){return t(e,"image")})}();