
(function(){var l,f,a,p,r,q,j;var e=null;function g(s){k(true);n();e=s.aid;j=true;r.setValueOf("info","href",s.ahref);r.setValueOf("info","target",s.atarget||"notSet");r.setValueOf("info","alt",s.aalt);r.setValueOf("info","title",s.atitle);j=false}function b(s){k(true);n();e=s;j=true;r.getContentElement("info","href").setValue("",true);r.getContentElement("info","target").setValue("notSet",true);r.getContentElement("info","alt").setValue("",true);r.getContentElement("info","title").setValue("",true);j=false}function h(){e=null;k(false)}function k(s){for(var u=1;u<=2;u++){var v=r.getContentElement("info","properties"+u),t=v.getElement();if(s){t.setStyle("visibility","")}else{t.setStyle("visibility","hidden")}}}function n(){if(e!==null){l.areas[e].ahref=r.getValueOf("info","href");l.areas[e].aalt=r.getValueOf("info","alt");l.areas[e].atitle=r.getValueOf("info","title")}}function o(s){if(s=="pointer"){l.is_drawing=0;l.nextShape="";q.$.style.cursor="default"}else{l.nextShape=s;q.$.style.cursor="crosshair"}m(s)}function m(s){if(p){p.removeClass("imgmapButtonActive")}p=r.getContentElement("info","btn_"+s).getElement();p.addClass("imgmapButtonActive")}function c(u){var t="";for(var s=0;s<u.areas.length;s++){t+=i(u.areas[s])}return t}function i(t){if(!t||t.shape===""){return""}var s='<area shape="'+t.shape+'" coords="'+t.lastInput+'"';if(t.aalt){s+=' alt="'+t.aalt+'"'}if(t.atitle){s+=' title="'+t.atitle+'"'}if(t.ahref){s+=' href="'+t.ahref+'" data-cke-saved-href="'+t.ahref+'"'}if(t.atarget&&t.atarget!="notSet"){s+=' target="'+t.atarget+'"'}s+="/>";return s}function d(){if(j){return}var s=e;if(s!==null){l.areas[s]["a"+this.id]=this.getValue();l._recalculate(s)}}CKEDITOR.dialog.add("ImageMaps",function(A){var v=A.lang.imagemaps,x=A.lang.common.generalTab,t="pic_container"+CKEDITOR.tools.getNextNumber(),y="StatusContainer"+CKEDITOR.tools.getNextNumber(),z=A.plugins.imagemaps,s=false;if(CKEDITOR.env.ie&&typeof window.CanvasRenderingContext2D=="undefined"){CKEDITOR.scriptLoader.load(z.path+"dialog/excanvas.js",D)}if(typeof imgmap=="undefined"){CKEDITOR.scriptLoader.load(z.path+"dialog/imgmap.js",D)}var B="",w=CKEDITOR.document.getHead().append("style");w.setAttribute("type","text/css");B+='.imgmapButton {cursor:pointer; background: url("'+z.path+'images/sprite.png") no-repeat top left; width: 16px; height: 16px; display:inline-block;}';B+=".imgmapButtonActive {outline:1px solid #666; background-color:#ddd;}";B+=".imgmap_label {cursor:default;}";B+="#"+t+" img {max-width:none; max-height:none;}";if(CKEDITOR.env.ie&&CKEDITOR.env.version<11){w.$.styleSheet.cssText=B}else{w.$.innerHTML=B}function D(){if(!s){return}if(typeof imgmap=="undefined"){return}if(CKEDITOR.env.ie&&typeof window.CanvasRenderingContext2D=="undefined"){return}e=null;a=null;f=A.getSelection().getSelectedElement();if(!f||!f.is("img")){if(A.widgets){var M=A.widgets.focused;if(M&&(M.name=="image2"||M.name=="image")){var I=M.element;if(I){if(I.getName()=="img"){f=I}else{var J=I.getElementsByTag("img");if(J.count()==1){f=J.getItem(0)}}}}}}if(!f||!f.is("img")){alert(v.msgImageNotSelected);r.hide();return}var G=f.data?f.data("cke-saved-src"):f.getAttribute("_cke_saved_src"),H=document.getElementById(t);if(!G){G=f.$.src}var O=CKEDITOR.document.getWindow().getViewPaneSize();var P=O.height-290;P=Math.max(P,315);H.style.maxHeight=P+"px";l=new imgmap({mode:"editor2",custom_callbacks:{onSelectArea:g,onRemoveArea:h,onStatusMessage:function(R){document.getElementById(y).innerHTML=R},onLoadImage:function(S){var T=S.getAttribute("width"),R=S.getAttribute("height");if(T){S.style.width=T+"px"}if(R){S.style.height=R+"px"}q=new CKEDITOR.dom.element(S);q.on("dragstart",function(U){U.data.preventDefault()})}},pic_container:H,bounding_box:false,lang:"",CL_DRAW_SHAPE:"#F00",CL_NORM_SHAPE:"#AAA",CL_HIGHLIGHT_SHAPE:"#F00"});l.loadStrings(imgmapStrings);f=f.$;l.loadImage(G,parseInt(f.style.width||f.width||0,10),parseInt(f.style.height||f.height||0,10));var K=f.getAttribute("usemap",2)||f.usemap;if(typeof K=="string"&&K!==""){K=K.substr(1);var Q=A.editable?A.editable().$:A.document.$,N=Q.getElementsByTagName("MAP");for(var L=0;L<N.length;L++){if(N[L].name==K||N[L].id==K){a=N[L];l.setMapHTML(a);r.setValueOf("info","MapName",K);break}}}l.config.custom_callbacks.onAddArea=b;if(a){l.blurArea(l.currentid);l.currentid=0;l.selectedId=0;g(l.areas[0]);l.highlightArea(0);o("pointer")}else{m("rect")}u();window.setTimeout(u,1000)}function E(){A.fire("saveSnapshot");if(f&&f.nodeName=="IMG"){f.removeAttribute("usemap",0);f.src=f.attributes["data-cke-saved-src"].value}if(a){a.parentNode.removeChild(a)}r.hide()}function u(){var H=parseInt(CKEDITOR.revision,10);if(!isNaN(H)&&H<7296&&CKEDITOR.skins&&A.config.filebrowserBrowseUrl){return}var J=r.parts.contents,I=J.getFirst().getFirst(),G=document.getElementById(t);G.style.width=(parseInt(J.$.style.width,10))+"px";G.style.height=parseInt(G.style.height,10)+(parseInt(J.$.style.height,10)-I.$.offsetHeight)+"px"}var F="fieldset",C=parseInt(CKEDITOR.revision,10);if(!isNaN(C)&&C<7296&&CKEDITOR.skins&&A.config.filebrowserBrowseUrl){F="vbox"}return{title:v.title,minWidth:500,minHeight:510,buttons:[{type:"button",label:v.imgmapBtnRemove,onClick:E},CKEDITOR.dialog.okButton,CKEDITOR.dialog.cancelButton],contents:[{id:"info",label:x,title:x,elements:[{type:F,label:v.imgmapMap,id:"ContainerMapName",hidden:true,children:[{id:"MapName",type:"text",label:v.imgmapMapName,labelLayout:"horizontal",onChange:function(){l.mapname=this.getValue()}}]},{type:F,label:v.imgmapMapAreas,children:[{type:"hbox",id:"button_container",style:"margin-bottom:10px",widths:["20px","18px","18px","18px","26px","230px","100px"],children:[{type:"html",id:"btn_pointer",onClick:function(){o("pointer")},html:'<span style="background-position: 0 -64px;" class="imgmapButton" title="'+v.imgmapPointer+'"></span>'},{type:"html",id:"btn_rect",onClick:function(){o("rect")},html:'<span style="background-position: 0 -128px;" class="imgmapButton" title="'+v.imgmapRectangle+'"></span>'},{type:"html",id:"btn_circle",onClick:function(){o("circle")},html:'<span style="background-position: 0 0;" class="imgmapButton" title="'+v.imgmapCircle+'"></span>'},{type:"html",id:"btn_poly",onClick:function(){o("poly")},html:'<span style="background-position: 0 -96px;" class="imgmapButton" title="'+v.imgmapPolygon+'"></span>'},{type:"html",onClick:function(){l.removeArea(l.currentid)},html:'<span style="background-position: 0 -32px;" class="imgmapButton" title="'+v.imgmapDeleteArea+'"></span>'},{type:"html",html:'<div id="'+y+'">&nbsp;</div>'},{type:"select",id:"zoom",labelLayout:"horizontal",label:v.imgmapLabelZoom,onChange:function(){var H=this.getValue();var G=document.getElementById(t).getElementsByTagName("img")[0];if(!G){return}if(!G.oldwidth){G.oldwidth=G.width}if(!G.oldheight){G.oldheight=G.height}G.style.width=G.oldwidth*H+"px";G.style.height=G.oldheight*H+"px";l.scaleAllAreas(H)},"default":"1",items:[["25%","0.25"],["50%","0.5"],["100%","1"],["200%","2"],["300%","3"]]}]},{type:"hbox",id:"properties1",style:"visibility:hidden",children:[{type:"text",id:"href",label:v.linkURL,onChange:d},{type:"button",id:"browse",label:A.lang.common.browseServer,style:"display:inline-block;margin-top:10px;",align:"center",hidden:"true",filebrowser:"info:href"},{id:"target",type:"select",label:v.linkTarget,onChange:d,items:[[v.notSet,"notSet"],[v.linkTargetSelf,"_self"],[v.linkTargetBlank,"_blank"],[v.linkTargetTop,"_top"]]}]},{type:"hbox",id:"properties2",style:"visibility:hidden",children:[{type:"text",id:"title",label:v.advisoryTitle,onChange:d},{type:"text",id:"alt",hidden:true,label:v.altText,onChange:d}]}]},{type:"fieldset",style:"border:0; padding:0",label:"&nbsp;",children:[{type:"html",html:'<div id="'+t+'" style="overflow:auto;width:500px;height:390px;position:relative;"></div>'}]}]}],onLoad:function(){r=this;r.on("resize",u)},onShow:function(){s=true;D()},onHide:function(){if(p){p.removeClass("imgmapButtonActive");p=null}document.getElementById(t).innerHTML=""},onOk:function(){n();if(f&&f.nodeName=="IMG"){var H=c(l);if(!H){E();return}l.mapid=l.mapname=r.getValueOf("info","MapName");var G=A.fire("imagemaps.validate",l);if(typeof G=="boolean"){return false}A.fire("saveSnapshot");H=c(l);if(!a){a=A.document.$.createElement("map");var I=f;if(A.widgets){var J=A.widgets.focused;if(J){I=J.wrapper.$}}I.parentNode.insertBefore(a,I.nextSibling)}a.innerHTML=H;if(a.name){a.removeAttribute("name")}a.name=l.getMapName();a.id=l.getMapId();f.setAttribute("usemap","#"+a.name,0);if(CKEDITOR.plugins.imagemaps&&CKEDITOR.plugins.imagemaps.drawMap){CKEDITOR.plugins.imagemaps.drawMap(f,a)}}}}})})();