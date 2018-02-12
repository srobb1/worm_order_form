<?php
$onto_term_submit  = $variables['node']->onto_term_submit;
?>
<div class="tripal_onto_term_submit-data-block-desc tripal-data-block-desc"></div> <?php

print "<h3>This is a summary of the term submission.</h3>
<ul>
  <li>The status will be updated as progress is made on the review and addition of the term into the ontology.</li>
  <li>Bookmark the URL of this page to return and check the status</li>
  <li><a href=\"/ontology-term-submission\">Click</a> here to view all submissions.</li>
</ul>";

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
// Type row
$rows[] = array(
  array(
    'data' => 'Worm Quantity',
    'header' => TRUE
  ),
  $onto_term_submit->worm_quantity
);
// Unique Name row
$rows[] = array(
  array(
    'data' => 'Biotype',
    'header' => TRUE
  ),
  $onto_term_submit->worm_biotype
);

// definition row
$rows[] = array(
  array(
    'data' => 'Definition',
    'header' => TRUE
  ),
  $onto_term_submit->definition
);


// Type row
$rows[] = array(
  array(
    'data' => 'Lab head',
    'header' => TRUE
  ),
  $onto_term_submit->contact_lab_head
);
// Type row
$rows[] = array(
  array(
    'data' => 'Contact Position',
    'header' => TRUE
  ),
  $onto_term_submit->contact_position
);

// Type row
$rows[] = array(
  array(
    'data' => 'Shipping Address',
    'header' => TRUE
  ),
  $onto_term_submit->sydelivery_addressns
);

// Type row
$rows[] = array(
  array(
    'data' => 'Shipping Phone',
    'header' => TRUE
  ),
  $onto_term_submit->delivery_phone
);

$rows[] = array(
  array(
    'data' => 'FedEx Account',
    'header' => TRUE
  ),
  $onto_term_submit->fedex_account
);

// Type row
$rows[] = array(
  array(
    'data' => 'Comments',
    'header' => TRUE
  ),
  $onto_term_submit->comments
);

// Type row
$rows[] = array(
  array(
    'data' => 'Contact Name',
    'header' => TRUE
  ),
  $onto_term_submit->contact_name
);

$rows[] = array(
  array(
    'data' => 'Receiver Name',
    'header' => TRUE
  ),
  $onto_term_submit->receiver_name
);


// Type row
$rows[] = array(
  array(
    'data' => 'Contact Email',
    'header' => TRUE
  ),
  $onto_term_submit->contact_email
);




// Type row
$statuses = array ('Worms Requested','Order being processed', 'Order Shipped');
$status = $statuses[$onto_term_submit->sstatus];
$rows[] = array(
  array(
    'data' => 'Status',
    'header' => TRUE
  ),
  $status
//  $onto_term_submit->status
);
// Type row
if (isset($user->roles[$admin_rid]) or isset($user->roles[$worm_order_form_admin_rid])){
$rows[] = array(
  array(
    'data' => 'Status Notes',
    'header' => TRUE
  ),
  $onto_term_submit->status_notes
);
}
// allow site admins to see the onto_term_submit ID
if (user_access('view ids')) {
  // Feature ID
  $rows[] = array(
    array(
      'data' => 'Order ID',
      'header' => TRUE,
      'class' => 'tripal-site-admin-only-table-row',
    ),
    array(
      'data' => $onto_term_submit->worm_order_form_id,
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
print theme_table($table); ?>
