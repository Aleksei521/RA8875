var TableInputs = new Array();
var NumItems = 0;



function DoAdd( form )
{
   var NewCell;
   var tbl = document.getElementById( "ZgateTableId" );
   var RowIdx = tbl.rows.length -1;
   var ColIdx = tbl.rows[ RowIdx ].cells.length;
   var x;


   if( +NumItems >= MaxTableSize )
   {
      alert( "List is full.  Delete unused entries." );
      return;
   }

   //alert( "Num Table rows " + RowIdx + " num cols " + ColIdx + "MaxCols " + 
   //   MaxCols + "MaxTableSize " + MaxTableSize + "+NumItems " + +NumItems );

   if( ColIdx == (2 * MaxCols) )
   {
      RowIdx++;
      var NewRow = tbl.insertRow( RowIdx );
      ColIdx = 0;
   }

   NewCell = tbl.rows[ RowIdx ].insertCell( ColIdx++ );
   NewCell.innerHTML = '<font size="1">' + ItemType + '_' + ++NumItems;

   NewCell = tbl.rows[ RowIdx ].insertCell( ColIdx );
   NewCell.innerHTML = '<' + NewEntryHTML + '>';
}



function RemoveCell( Idx )
{
   var tbl = document.getElementById( "ZgateTableId" );
   var ColIdx = Idx % MaxCols;
   var RowIdx = (Idx - ColIdx) / MaxCols;


   ColIdx = ColIdx * 2;
   //alert( "empty cell " + Idx + " " + RowIdx + "," + ColIdx );
   tbl.rows[ RowIdx ].deleteCell( ColIdx );
   tbl.rows[ RowIdx ].deleteCell( ColIdx );

   if( (ColIdx == 0) && RowIdx )
   {
      tbl.deleteRow( RowIdx );
   }
   TableInputs.splice( Idx, 1 );
   NumItems--;
}



function DoCancel( form ) 
{	
   var el = document.getElementsByName( "InputValue" );


   for( var i=el.length; i; )
   {
      i--;
      if( i < TableInputs.length )
      {
         el[i].value = TableInputs[i];
      }
      else
      {
         RemoveCell( i );
      }
   }
}



function AddToSubmitForm( Op, Item ) 
{
   var form;
   var el;


   form = document.getElementById( "SubmitForm" );
   if( form == null )
   {
      //alert( "form does not exist" );
      form = document.createElement( "form" ); 
      form.setAttribute( "method", "post" ); 
      form.setAttribute( "action", "/ZGATE_Conf" ); 
      form.setAttribute( "id", "SubmitForm" ); 
   }

   el = document.createElement( "input" );
   el.setAttribute( "type", "hidden" );
   el.setAttribute( "name", ZgateCmdStr );
   el.setAttribute( "value", Op + " " + Item );
   form.insertBefore( el, form.firstChild );
   document.body.appendChild( form ); 
}



function PostSubmitForm( )
{
   var form = document.getElementById( "SubmitForm" );
   form.submit();
}



function DoSubmit( form ) 
{	
   var el = document.getElementsByName( "InputValue" );
   var Value;
   var CallCancel = 0;
   var Changes = 0;


   /*
    * Add placeholders in the TableInputs array to ensure it is at least as long
    * as the array of table elements.
    */ 
   while( el.length > TableInputs.length )
   {
      TableInputs.push( "" );
   }

   for( var i=el.length; i; )
   {
      i--;

      if( el[i].value != "" )
      {
         if( IsEntryValid(el[i].value) == 0 )
         {
            alert( "Invalid entry " + el[i].value );
            break;
         }
      }

      if( el[i].value == TableInputs[i] )
      {
         continue;
      }
      if( el[i].value == "" )
      {
         //alert( "Del port " + TableInputs[i] + " Idx " + i);
         AddToSubmitForm( "del", TableInputs[i] );
         Changes++;
         TableInputs.splice( i, 1 );
         NumItems--;
         CallCancel = 1;
      }
      else
      {
         if( TableInputs[i] == "" )
         {
            //alert( "add port " + el[i].value + " Idx " + i );
            TableInputs.push(  el[i].value );
            NumItems++;
            AddToSubmitForm( "add", el[i].value );
            Changes++;
         }
         else
         {
            var temp = TableInputs[i];

            //alert( "Del port " + TableInputs[i] + " Add port " + el[i].value + " Idx " + i);
            TableInputs[i] = el[i].value;
            AddToSubmitForm( "add", el[i].value );
            Changes++;
            AddToSubmitForm( "del", temp );
            Changes++;
         }
      }
   }
   if( CallCancel == 1 )
   {
      DoCancel( form );
   }
   if( Changes )
   {
      PostSubmitForm();
   }
}



function GenerateTable( )
{
   document.write( '' +

      '<style TYPE="text/css"> ' +
      ' body ' +
      ' { ' +
      '   color:#000000; ' +
      '   font-family:Verdana; ' +
      ' } ' +
      ' table ' +
      ' { ' +
      '    font-size:14px; ' +
      ' } ' +
      ' h6 ' +
      ' { ' +
      '   color:#004400; ' +
      '   font-size:16px; ' +
      ' } ' +
      '</style> ' +

      '<div><table border="0" width="600">' +
      '  <tr><td colspan="2">' +
      '    <br>' +
      '    <b><h6>ZGATE&#153 ' + TableType + ' Configuration</h6>' +
      '  </td></tr>' +
      '  <tr><td>' +
      '    Filtering Mode:</td><td><b>' + FilterMode + '</b>' +
      '  </td></tr>' +
      '  <tr><td>' +
      '    Maximum List Size:</td><td><b>' + MaxTableSize + '</b>' +
      '  </td></tr>' +

      '  <tr><td colspan="2">' + '<br>' );
   if( FilterMode == "Whitelist" )
   {
     document.write( WL_Desc );
   }
   if( FilterMode == "Blacklist" )
   {
     document.write( BL_Desc );
   }
   if( FilterMode == "Disabled" )
   {
     document.write( Disabled_Desc );
   }
   else
   {
      document.write( '<tr><td colspan="2"><br>' +
      '  To modify an entry, edit the value(s) displayed in one or more of the boxes below and click the "Update" button.' +
      '  To remove an entry, delete one or more of the items in the list and click the "Update" button.' +
      '  To add a new item click the "Add" button, type in the new value and click the "Update" button.' +
      '  To discard changes since the last time the page was refreshed (or the "Update" button was clicked) click the "Cancel" button.' +
      '  </td></tr>' );
   }
   document.write( '</td></tr></table></div>' );


   if( (FilterMode != "Disabled") && (MaxTableSize > 0) )
   {
      NumItems = 0;
      document.write("<FORM>")

      document.write( '<table border="0" name="MyTable" id="ZgateTableId" width="600">');

      if( PortList != "" )
      {
         TableInputs = PortList.split(",");
      }

      document.write( '<tr>' );

      for( x in TableInputs )
      {
         NumItems++

         document.write('<td><font size="1">' + ItemType + '_' + NumItems + '</td><td><' + NewEntryHTML + ' VALUE=' + TableInputs[x] + '></td>' );
         if( NumItems % MaxCols == 0 )
         {
            document.write('</tr><tr>' );
         }
      }
      document.write( '</tr></table>' )

      document.write('<table border="0" width="600"><tr align="center"><br>' );
      document.write('<td width="200"><INPUT TYPE="button" VALUE="  Add  "    onClick="DoAdd(this.form)"></td>')
      document.write('<td width="200"><INPUT TYPE="button" VALUE="Update" onClick="DoSubmit(this.form)"></td>')
      document.write('<td width="200"><INPUT TYPE="button" VALUE="Cancel" onClick="DoCancel(this.form)"></td>')
      document.write('</tr></table>' );

      document.write( "</FORM>" );
   }
}

