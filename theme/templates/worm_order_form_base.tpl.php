<?php
$worm_order_form  = $variables['node']->worm_order_form;
?>
<div class="tripal_worm_order_form-data-block-desc tripal-data-block-desc"></div> <?php


  global $user;
  $admin_rid = user_role_load_by_name('administrator')->rid; 
  $worm_order_form_admin_rid = user_role_load_by_name('worm_order_form_admin')->rid; 

// the $headers array is an array of fields to use as the column headers.
// additional documentation can be found here
// https://api.drupal.org/api/drupal/includes%21theme.inc/function/theme_table/7
// This table for the analysis has a vertical header (down the first column)
// so we do not provide headers here, but specify them in the $rows array below.
$headers = array();
// the $rows array contains an array of rows where each row is an array
// of values for each column of the table in that row. Additional documentation
// can be found here:
// https://api.drupal.org/api/drupal/includes%21theme.inc/function/theme_table/7
$rows = array();
$rows[] = array(
  array(
    'data' => 'Order Date',
    'header' => TRUE
  ),
  $worm_order_form->request_date
);

$rows[] = array(
  array(
    'data' => 'Worm Quantity',
    'header' => TRUE
  ),
  $worm_order_form->worm_quantity
);
// Unique Name row
//$rows[] = array(
//  array(
//    'data' => 'Biotype',
//    'header' => TRUE
//  ),
//  $worm_order_form->biotype
//);


// Type row
$rows[] = array(
  array(
    'data' => 'Lab head',
    'header' => TRUE
  ),
  $worm_order_form->contact_lab_head
);
// Type row
$rows[] = array(
  array(
    'data' => 'Contact Position',
    'header' => TRUE
  ),
  $worm_order_form->contact_position
);

// Type row
$rows[] = array(
  array(
    'data' => 'Shipping Address',
    'header' => TRUE
  ),
  $worm_order_form->delivery_address
);

// Type row
$rows[] = array(
  array(
    'data' => 'Shipping Phone',
    'header' => TRUE
  ),
  $worm_order_form->delivery_phone
);

$rows[] = array(
  array(
    'data' => 'FedEx Account',
    'header' => TRUE
  ),
  $worm_order_form->fedex_account
);

// Type row
$rows[] = array(
  array(
    'data' => 'Comments',
    'header' => TRUE
  ),
  $worm_order_form->comments
);

// Type row
$rows[] = array(
  array(
    'data' => 'Contact Name',
    'header' => TRUE
  ),
  $worm_order_form->contact_name
);

$rows[] = array(
  array(
    'data' => 'Receiver Name',
    'header' => TRUE
  ),
  $worm_order_form->receiver_name
);


// Type row
$rows[] = array(
  array(
    'data' => 'Contact Email',
    'header' => TRUE
  ),
  $worm_order_form->contact_email
);




// Type row
$statuses = array ('Worms Requested','Order Being Processed', 'Order Shipped' , 'Issues With Order' , 'Order Declined');
$status = $statuses[$worm_order_form->sstatus];
$rows[] = array(
  array(
    'data' => 'Status',
    'header' => TRUE
  ),
  $status
//  $worm_order_form->status
);
// Type row
if (isset($user->roles[$admin_rid]) or isset($user->roles[$worm_order_form_admin_rid])){
$rows[] = array(
  array(
    'data' => 'Status Notes',
    'header' => TRUE
  ),
  $worm_order_form->status_notes
);
$rows[] = array(
  array(
    'data' => 'Date Shipped',
    'header' => TRUE
  ),
  $worm_order_form->date_shipped
);
$rows[] = array(
  array(
    'data' => 'Shipper Name',
    'header' => TRUE
  ),
  $worm_order_form->shipper_name
);
$rows[] = array(
  array(
    'data' => 'Airbill',
    'header' => TRUE
  ),
  $worm_order_form->airbill
);
}
// allow site admins to see the worm_order_form ID
if (user_access('view ids')) {
  // Feature ID
  $rows[] = array(
    array(
      'data' => 'Order ID',
      'header' => TRUE,
      'class' => 'tripal-site-admin-only-table-row',
    ),
    array(
      'data' => $worm_order_form->worm_order_form_id,
      'class' => 'tripal-site-admin-only-table-row',
    ),
  );
}
// the $table array contains the headers and rows array as well as other options
// for controlling the display of the table. Additional documentation can be
// found here:
// https://api.drupal.org/api/drupal/includes%21theme.inc/function/theme_table/7
$table = array(
  'header' => $headers,
  'rows' => $rows,
  'attributes' => array(
    'id' => 'tripal_worm_order_form-table-base',
    'class' => 'tripal-data-table'
  ),
  'sticky' => FALSE,
  'caption' => '',
  'colgroups' => array(),
  'empty' => '',
);
// once we have our table array structure defined, we call Drupal's
// theme_table() function to generate the table.
print theme_table($table); 
$path = drupal_get_path('module', 'worm_order_form');
print "<b><a href=\"/search/worm-order-form.csv?worm_order_form_id=$worm_order_form->worm_order_form_id&contact_name=All&contact_lab_head=&sstatus=All\"><img src=\"/$path/theme/img/download_csv.png\"></a>";
?>
