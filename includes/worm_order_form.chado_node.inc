<?php

/**
 * @file
 * This file should contain all Drupal hooks for interacting with nodes.
 *
 */

/**
 *  Implementation of hook_node_info().
 *
 *  This hook provides information to Drupal about any node types that are being
 *  created by this module. If your module does not create any node types then
 *  this function is not required.
 *
 * @ingroup worm_order_form
 */
function worm_order_form_node_info() {
  $nodes = array();

  // EXPLANATION: this array describes all of the node types that are created
  // by this module. For many Tripal modules (e.g. worm_order_form, tripal_stock,
  // tripal_library, tripal_pub, etc.) new node types are created. It is
  // customary to name all new node types that interact with data in Chado
  // with a 'chado_' prefix.

  $nodes['chado_worm_order_form'] = array(
    'name'        => t('Worm Order'),
    'base'        => 'chado_worm_order_form',
    'description' => t('A record from the fake chado worm_order_form table'),
    'has_title'   => TRUE,
    'locked'      => TRUE,
    // EXPLANATION: This section of the node type array specifies how Tripal
    // will sync the node types with data in Chado. When Drupal creates a node
    // it has no way of coordinating which node belongs to which record in
    // Chado. Therefore, Tripal maintains tables in the Drupal schema that maps
    // Drupal nodes to records in Chado. Syncing is the process of creating
    // Drupal nodes and linking them to the appropriate record.
    'chado_node_api' => array(
      // the base table name (e.g. worm_order_form, worm_order_form, contact)
      'base_table' => 'worm_order_form',
      // the node type hook prefix
      'hook_prefix' => 'chado_worm_order_form',
      'record_type_title' => array(
        // how to refer to the record
        'singular' => t('Worm Order Submission'),
        // how to refer to the record in plurals
        'plural' => t('Worm Order Submissions')
      ),
      'sync_filters' => array(
        'type_id' => FALSE,     // if the record has a type_id set to TRUE
        'organism_id' => FALSE  // if the record has an organism_id set to TRUE
      ),
    )
  );

  return $nodes;
}

/**
 * Implement hook_access(). This hook provides instructions to Drupal for which
 * users can access the custom content types created in the function above. The
 * available permissions are set in the chado_worm_order_form_permissions() hook in the
 * worm_order_form.module file. This hook is not needed if no node types were
 * defined in the hook_node_info() hook.
 *
 * @return
 * This function should return null if it does not specifically deny access.
 * This allows for other mechanisms to to deny or reject access. If the return
 * value is TRUE then access is granted regardless of any other rules that might
 * be implemented by other modules.
 */
function worm_order_form_node_access($node, $op, $account) {
  $node_type = $node;
  if (is_object($node)) {
    $node_type = $node->type;
  }
  // EXPLANATION:  in the worm_order_form_permissions() function we created the
  // permission types that are used here to check for access permissions to the
  // 'chado_exmaple' node type.
  if($node_type == 'chado_worm_order_form') {
    if ($op == 'create') {
      if (!user_access('create chado_worm_order_form content', $account)) {
        return NODE_ACCESS_DENY;
      }
      return NODE_ACCESS_ALLOW;
    }
    if ($op == 'update') {
      if (!user_access('edit chado_worm_order_form content', $account)) {
        return NODE_ACCESS_DENY;
      }
    }
    if ($op == 'delete') {
      if (!user_access('delete chado_worm_order_form content', $account)) {
        return NODE_ACCESS_DENY;
      }
    }
    if ($op == 'view') {
      if (!user_access('access chado_worm_order_form content', $account)) {
        return NODE_ACCESS_DENY;
      }
    }
  }
  return NODE_ACCESS_IGNORE;
}

/**
 * Implementation of hook_form()
 *
 * Creates the form for editing or inserting a record
 *
 * @ingroup worm_order_form
 */
function chado_worm_order_form_form($node, &$form_state) {
  // EXPLANATION: This function should construct a form array that is used by
  // Drupal to construct a form for inserting or editing our new node type.
  // See this page for information about the Form API:
  // https://api.drupal.org/api/drupal/includes!form.inc/group/form_api/7
  //
  // The code below is laid out in the following order
  // 1) Set default values
  // 2) Add form elements used by this node type
  // 3) Use the Tripal API to add form elements for properties,
  //    dbxref's and relationships
  //
  // For the worm_order_form code below we assume that the fake 'worm_order_form' table only has
  // a uniquename, organism_id, type_id and worm_order_form_id.

  global $user;
  $form = array();
  $admin_rid = user_role_load_by_name('administrator')->rid; 
  $worm_order_form_admin_rid = user_role_load_by_name('worm_order_form_admin')->rid; 


  // Default values can come in the following ways:
  //
  // 1) as elements of the $node object. This occurs when editing an existing
  //    worm_order_form
  // 2) in the $form_state['values'] array which occurs on a failed validation
  //    or ajax callbacks from non submit form elements
  // 3) in the $form_state['input'[ array which occurs on ajax callbacks from
  //    submit form elements and the form is being rebuilt
  //
  // set form field defaults


  // SET FORM DEFAULTS
  //---------------------------------------------
  $worm_order_form      = null; // holds the worm_order_form object record
  $worm_order_form_id   = null; // when editing an worm_order_form record we'll have an worm_order_form_id

  // initialize the defaults for the form fields
  $worm_quantity   = '';
  $worm_biotype   = 0;
  $contact_name  = '';
  $contact_email = '';
  $contact_position = '';
  $contact_lab_head = '';
  $delivery_address = '';
  $delivery_phone = '';
  $receiver_name = '';
  $carrier = '';
  $fedex_account = '';
  $condition_acknowledgement = '';
  $comments = '';
  $customs_file_upload = '';
  $sstatus = '' ;
  $date_shipped = '';
  $airbill = '' ;
  $shipper_name = '' ;
  $status_notes= '';

  // if we are editing an existing node then the 'worm_order_form' record from Chado
  // is already part of the node, so we set the defaults from that object
  if (property_exists($node, 'worm_order_form')) {
    $worm_order_form = $node->worm_order_form;
    $worm_order_form_id   = $worm_order_form->worm_order_form_id;
    $worm_quantity   = $worm_order_form->worm_quantity;
    $worm_biotype  = $worm_order_form->worm_biotype;
    $contact_lab_head = $worm_order_form->contact_lab_head;
    $contact_position = $worm_order_form->contact_position;
    $delivery_address  = $worm_order_form->delivery_address;
    $delivery_phone = $worm_order_form->delivery_phone;
    $fedex_account = $worm_order_form->fedex_account;
    $carrier = $worm_order_form->carrier;
    $condition_acknowledgement = $worm_order_form->condition_acknowledgement;
    $receiver_name  = $worm_order_form->receiver_name;
    $contact_name  = $worm_order_form->contact_name;
    $contact_email = $worm_order_form->contact_email;
    $comments = $worm_order_form->comments;
    $customs_file_upload = $worm_order_form->customs_file_upload;
    $sstatus = $worm_order_form->sstatus;
    $date_shipped = $worm_order_form-> date_shipped;
    $airbill = $worm_order_form-> airbill;
    $shipper_name = $worm_order_form-> shipper_name;
    $status_notes = $worm_order_form->status_notes;

    // keep track of the worm_order_form id
    $form['worm_order_form_id'] = array(
      '#type' => 'value',
      '#value' => $worm_order_form_id,
    );
  }
  // if we are re constructing the form from a failed validation or ajax
  // callback then use the $form_state['values'] values
  if (array_key_exists('values', $form_state)) {
     $worm_quantity   = $form_state['values']['worm_quantity'];
     $worm_biotype  = $form_state['values']['worm_biotype'];
     $contact_lab_head  = $form_state['values']['contact_lab_head'];
     $contact_position = $form_state['values']['contact_position'];
     $delivery_address  = $form_state['values']['delivery_address'];
     $delivery_phone = $form_state['values']['delivery_phone'];
     $fedex_account  = $form_state['values']['fedex_account'];
     $carrier  = $form_state['values']['carrier'];
     $condition_acknowledgement  = $form_state['values']['condition_acknowledgement'];
     $receiver_name  = $form_state['values']['receiver_name'];
     $contact_name  = $form_state['values']['contact_name'];
     $contact_email  = $form_state['values']['contact_email'];
     $comments  = $form_state['values']['comments'];
     $customs_file_upload  = $form_state['values']['customs_file_upload'];
     $sstatus  = $form_state['values']['sstatus'];
     $date_shipped  = $form_state['values']['date_shipped'];
     $airbill  = $form_state['values']['airbill'];
     $shipper_name  = $form_state['values']['shipper_name'];
     $status_notes  = $form_state['values']['status_notes'];
  }
  // if we are re building the form from after submission (from ajax call) then
  // the values are in the $form_state['input'] array
  if (array_key_exists('input', $form_state) and !empty($form_state['input'])) {
     $worm_quantity   = $form_state['input']['worm_quantity'];
     $worm_biotype  = $form_state['input']['worm_biotype'];
     $contact_lab_head  = $form_state['input']['contact_lab_head'];
     $contact_position = $form_state['input']['contact_position'];
     $delivery_address  = $form_state['input']['delivery_address'];
     $delivery_phone = $form_state['input']['delivery_phone'];
     $fedex_account  = $form_state['input']['fedex_account'];
     $carrier  = $form_state['input']['carrier'];
     $condition_acknowledgement  = $form_state['input']['condition_acknowledgement'];
     $receiver_name  = $form_state['input']['receiver_name'];
     $contact_name  = $form_state['input']['contact_name'];
     $contact_email  = $form_state['input']['contact_email'];
     $comments  = $form_state['input']['comments'];
     $customs_file_upload  = $form_state['input']['customs_file_upload'];
     $sstatus  = $form_state['input']['sstatus'];
     $date_shipped  = $form_state['input']['date_shipped'];
     $airbill  = $form_state['input']['airbill'];
     $shipper_name  = $form_state['input']['shipper_name'];
     $status_notes  = $form_state['input']['status_notes'];
  }


  // FORM ELEMENTS
  //---------------------------------------------
  $form['worm_quantity'] = array(
    '#type' => 'textfield',
    '#title' => t('Worm Quantity'),
    '#required' => TRUE,
    '#default_value' => $worm_quantity,
    '#description' => t('REQUIRED. Enter a the number of worms that you would like to have shipped. Example values are: 500, 1000, 2000'), 
    '#maxlength' => 255
  );
  $form['worm_biotype'] = array(
    '#title'       => t('Worm Biotype'),
    '#type' => 'hidden', 
    '#value' => $worm_biotype,
//    '#type'        => t('select'),
//    '#description' => t("REQUIRED. Select the biotype."),
//    '#required'    => TRUE,
//    '#options' => array( 0 => "Asexual" ),
//    '#default_value' => $worm_biotype
  );



$form['contact_lab_head'] = array(
    '#type' => 'textfield',
    '#title' => t('Lab Head'),
    '#required' => TRUE,
    '#default_value' => $contact_lab_head ,
    '#description' => t('REQUIRED. Enter the name of your lab head and the requestor\'s insitute.'),

  ); 



  $form['contact_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Name'),
    '#required' => TRUE,
    '#default_value' => $contact_name,
    '#description' => t('REQUIRED. Enter Your Name.'),
  );
  $form['contact_position'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Position'),
    '#required' => TRUE,
    '#default_value' => $contact_position,
    '#description' => t('REQUIRED.Your Position. Examples: Posdoc, Grad Student, Technician, PI'),
  );

  $form['contact_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Your E-mail address'),
    '#required' => TRUE,
    '#default_value' => $contact_email,
    '#description' => t('REQUIRED. Enter your email address for us to contact you about this submission.'),
  );



  $form['receiver_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Shipping  Name'),
    '#required' => TRUE,
    '#default_value' => $receiver_name,
    '#description' => t('REQUIRED. To whom should the package be addressed?'),
  );
  $form['delivery_address'] = array(
    '#type' => 'text_format',
    '#title' => t('Shipping Address'),
    '#required' => TRUE,
    '#default_value' => $delivery_address,
    '#description' => t('REQUIRED'), 
  );
  $form['delivery_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Shipping Phone Number'),
    '#required' => TRUE,
    '#default_value' => $delivery_phone,
    '#description' => t('REQUIRED. Needed for FedEx delivery.'), 
  );

  $select_carrier = array(0 => t('FedEx (Preferred)'), 1 => t('UPS') , 2 => t('Other (Specify in comments below)'));
  $form['carrier'] = array(
    '#type' => 'select',
    '#title' => t('Select Carrier'),
    '#default_value' => $carrier,
    '#options' => $select_carrier,
    '#description' => t('REQUIRED. Please select a carrier. FedEx is our preferred carrier.'),
    '#required' => TRUE
  );

  $form['fedex_account'] = array(
    '#type' => 'textfield',
    '#title' => t('Shipping Carrier Account Number'),
    '#default_value' => $fedex_account,
    '#description' => t('REQUIRED. Please prefix account number with FEDEX or UPS. Account number will be used to cover shipping costs'),
    '#required' => TRUE,
    '#maxlength' => 255
  );

  
  $form['customs_file_upload'] = array(
    '#type' => 'file',
    '#title' => t('Choose a file'),
    '#default_value' => $customs_file_upload,
    '#description' => t('REQUIRED if shipping Internationally. For international shipments, please contact your Customs agency to obtain the necessary permit, paperwork, or other import clearance documentation. If no permits are required please supply a letter or document from the customs agency indicating that no accompanying documentation is required.' ),
  );

  $form['comments'] = array(
    '#type' => 'text_format',
    '#title' => t('Any Comments'),
    '#default_value' => $comments,
    '#description' => t('NOT REQUIRED. Enter any comments about this term or submission.' ),
  );

if (isset($user->roles[$admin_rid]) or isset($user->roles[$worm_order_form_admin_rid])){
  $form['sstatus'] = array(
    '#type' => t('select'),
    '#title' => t('Status'),
    '#options' => array(0 => 'Worms Requested',1 => 'Order Being Processed', 2 => 'Order Shipped' , 3 => 'Issues With Order' , 4 => 'Order Declined'),
    '#default_value' => $sstatus,
    '#description' => t('Select the appropriate term to indicate the workflow progress.'),

  );
  $form['date_shipped'] = array(
    '#type' => t('textfield'),
    '#title' => t('Date Shipped'),
    '#default_value' => $date_shipped,
    '#description' => t('Enter shipping date.'),
  );
  $form['airbill'] = array(
    '#type' => t('textfield'),
    '#title' => t('Airbill ID'),
    '#default_value' => $date_shipped,
    '#description' => t('Enter Shipment Airbill ID.'),
  );
  $form['shipper_name'] = array(
    '#type' => t('textfield'),
    '#title' => t('Shipper Name'),
    '#default_value' => $shipper_name,
    '#description' => t('Enter name of the shipper.'),
  );
  $form['status_notes'] = array(
    '#type' => 'text_format',
    '#title' => t('Status Notes'),
    '#default_value' => $status_notes,
    '#description' => t('Enter any notes about workflow progress.'),
  );
}
  $form['condition_acknowledgement'] = array(
    '#type' => 'checkbox',
    '#title' => t('Agree to Terms and Conditions'),
    '#required' => TRUE,
    '#default_value' => $condition_acknowledgement,
    '#description' => t('REQUIRED. For your request to be submittted you must agree to the terms and conditions. <a target="_blank"  href="/worm_request"><strong>Review Terms and Conditions</strong></a><br><br><h3>Only Click Save One Time When Form is Complete.</h3>'),
  );

  // PROPERTIES FORM
  //---------------------------------------------
  // If there is a worm_order_formprop table and you want to allow users to add/remove
  // entries from it through your node form then add this section to your own
  // node form
  //$prop_cv = tripal_get_default_cv('worm_order_formprop', 'type_id');
  //$cv_id = $prop_cv ? $prop_cv->cv_id : NULL;
  $details = array(
  );
  // Adds the form elements to your current form

  // ADDITIONAL DBXREFS FORM
  //---------------------------------------------
  // If there is a worm_order_form_dbxref table and you want to allow users to
  // add/remove entries from it through your node form then add this section to
  // your own node form

  // RELATIONSHIPS FORM
  //---------------------------------------------
  // If there is a worm_order_form_relationship table and you want to allow users to
  // add/remove entries from it through your node form then add this section to
  // your own node form

 $form['#after_build'][] = 'worm_order_form_customize_comment_form';
  return $form;
}


function worm_order_form_customize_comment_form(&$form) {
  foreach (array('delivery_address', 'comments' , 'status_notes') as $field){

    $form[$field]['format']['format']['#access']=FALSE;
    $form[$field]['format']['format']['#default_value']='plain_text';
    $form[$field]['format']['help']['#access']=FALSE;
    $form[$field]['format']['guidelines']['#access']=FALSE;
  }
  //$form['defintion']['und'][0]['format']['#access'] = FALSE;
  return $form;
}



/**
 * Implementation of hook_validate
 *
 * This function validates a form prior to insert or update. If an error is
 * detected, it sets the error using form_set_error() which takes the user back
 * to the form to make corrections.
 *
 * This validation is being used for three activities:
 *   CASE A: Update a node that exists in both Drupal and Chado
 *   CASE B: Synchronizing a node from Chado to Drupal
 *   CASE C: Inserting a new node that exists in neither Drupal nor Chado
 *
 * @param $node
 *
 *
 * @ingroup worm_order_form
 */
function chado_worm_order_form_validate($node, $form, &$form_state) {
  // We only want to validate when the node is saved.
  // Since this validate can be called on AJAX and Deletion of the node
  // we need to make this check to ensure queries are not executed
  // without the proper values.
  if(property_exists($node, "op") and $node->op != 'Save') {
    return;
  }

  // we are syncing if we do not have a node ID but we do have a worm_order_form_id. We
  // don't need to validate during syncing so just skip it.
  if (!property_exists($node, 'nid') and property_exists($node, 'worm_order_form_id') and $node->worm_order_form_id != 0) {
    return;
  }
  // be sure to always trim text fields
  $node->contact_name   = property_exists($node, 'contact_name') ? trim($node->contact_name) : '';
  // Validating for an update. If the 'nid' property is present in the node then
  // this is an update and validation can be different for updates
  if (property_exists($node, 'nid')) {
    if(!valid_email_address($node->contact_email)){
      form_set_error('worm_order_form_type', t("The email address does not seem correct."));
    }
    $phone_ok = array();
    preg_match("/^([ +\d\-\(\)]+)$/", $node->delivery_phone, $phone_ok);
    if (empty($phone_ok)){
      form_set_error('worm_order_form_type', t("The contact phone number does not seem correct."));
    }

    $node->worm_quantity = str_replace(',', '', $node->worm_quantity);
    if(!is_numeric($node->worm_quantity)){
       form_set_error('worm_order_form_type', t("The provided worm quantity does not seem correct."));
    }


    $account_ok = array();
    preg_match("/^([ +\w\d\-\(\)]+)$/", $node->fedex_account, $account_ok);
    if (empty($account_ok)){
      form_set_error('worm_order_form_type', t("The carrier account number does not seem correct."));
    }
    // make sure the worm_order_form type is an allowed term
    //$type_cv = tripal_get_default_cv('worm_order_form', 'type_id');
    //$type = tripal_get_cvterm(array(
     // 'name' => $node->worm_order_form_type,
     // 'cv_id' => $type_cv->cv_id,
   // ));
   // if (!$type) {
    //  form_set_error('worm_order_form_type', t("The worm_order_form type is not a valid name from the Sequence Ontology."));
   // }

    // TODO: also we should check that the unique constraint is not invalidated
    // by changing either the type_id, organism_id or uniquename.
  }
  // Validating for an insert
  else {
    if(!valid_email_address($node->contact_email)){
      form_set_error('worm_order_form_type', t("The email address does not seem correct."));
    }
    $phone_ok = array();
    preg_match("/^([ +\d\-\(\)]+)$/", $node->delivery_phone, $phone_ok);
    if (empty($phone_ok)){
      form_set_error('worm_order_form_type', t("The contact phone number does not seem correct."));
    }
    $account_ok = array();
    preg_match("/^([ +\d\-\(\)]+)$/", $node->fedex_account, $account_ok);
    if (empty($account_ok)){
      form_set_error('worm_order_form_type', t("The carrier account number does not seem correct."));
    }

    $node->worm_quantity = str_replace(',', '', $node->worm_quantity);
    if(!is_numeric($node->worm_quantity)){
       form_set_error('worm_order_form_type', t("The provided worm quantity does not seem correct."));
    }
    // make sure the worm_order_form type is an allowed term
    //$type_cv = tripal_get_default_cv('worm_order_form', 'type_id');
   // $type = tripal_get_cvterm(array(
     // 'name' => $node->worm_order_form_type,
    //  'cv_id' => $type_cv->cv_id,
   // ));
    //if (!$type) {
   //   form_set_error('worm_order_form_type', t("The worm_order_form type is not a valid name from the Sequence Ontology."));
   // }
    // TODO: also we should check that the unique constraint doesn't already exist
  }
}

/**
 *  Implementation of hook_insert(). This function is called after the node is
 * inserted into the database. We need it so that we can insert appropriate
 * fields as provided by the user into the database. And so that we can link the
 * new Drupal node to the data in Chado via the chado_worm_order_form linking table. We
 * can get to this function also during "syncing".
 * With syncing, however, the data already exists in Chado and we do not want
 * to try to re-add it. But we do need to add an entry to the chado_worm_order_form
 * table to link the Drupal node with the data in the 'worm_order_form' table of Chado.
 *
 *  This function is not required if the hook_node_info() does not define
 *  any custom node types.
 *
 * @ingroup worm_order_form
 */
function chado_worm_order_form_insert($node) {
  $worm_order_form_id = '';

  // if there is an worm_order_form_id in the $node object then this must be a sync so
  // we can skip adding the worm_order_form as it is already there, although we do need
  // to proceed with insertion into the chado/drupal linking table.
  if (!property_exists($node, 'worm_order_form_id')) {

    // be sure to always trim text fields
    $node->worm_quantity = str_replace(',', '', $node->worm_quantity);
    $node->worm_quantity   = trim($node->worm_quantity);
    //$node->description  = property_exists('description', $node) : trim($node->description['value']) ? '';
    // get the worm_order_form type record
    // perform the insert using the chado_insert_record function();
     $biotype = 'Schmidtea mediterranea Asexuals';
     if ($node->worm_biotype == 1){
        $biotype = 'Schmidtea mediterranea Sexuals';
     }

     $node->carrier   = trim($node->carrier);
     $carrier_name = 'FedEx';
     if ($node->carrier == 1){
       $carrier_name = 'UPS';
     }elseif ($node->carrier == 2){
       $carrier_name = 'Other';
     }
    $filepath = 'public://customs_file_upload'; 
    file_prepare_directory($filepath, FILE_CREATE_DIRECTORY);
    $safe_files = array('file_validate_extensions' => array('jpg jpeg gif png txt doc docx xls xlsx pdf ppt pps odt ods odp'));
    $file = file_save_upload('customs_file_upload',$safe_files, $filepath);

//file upload test
if ($file){
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
}
// end test


    $file_url = file_create_url($file->uri);
    $values = array(
      'worm_quantity' => $node->worm_quantity,
      'worm_biotype' => $node->worm_biotype,
      'contact_lab_head' => $node->contact_lab_head,
      'contact_position' => $node->contact_position,
      'delivery_address' => trim(preg_replace('/\s\s+/', ' ', $node->delivery_address['value'])),//  trim($node->delivery_address['value']),
      'delivery_phone' => $node->delivery_phone,
      'fedex_account' => $node->fedex_account,
      'carrier' => $node->carrier,
      'carrier_name' => $carrier_name,
      'condition_acknowledgement' => $node-> condition_acknowledgement,
      'receiver_name' => $node->receiver_name,
      'contact_name' => $node->contact_name,
      'contact_email' => $node->contact_email,
      'comments' => $node->comments['value'],
      'customs_file_upload' => $file_url,
      'sstatus' => $node->sstatus ? defined($node->sstatus) : 0,
      'date_shipped' => $node->date_shipped,
      'airbill' => $node->airbill,
      'shipper_name' => $node->shipper_name,
      //'date_shipped' => $node->comments['date_shipped'],
      //'airbill' => $node->comments['airbill'],
      //'shipper_name' => $node->comments['shipper_name'],
      'status_notes' => $node->status_notes['value'],
      'request_date' => format_date($node->created, 'custom', "Y-m-d"), 
      'biotype' => $biotype,
    );
    $worm_order_form = chado_insert_record('worm_order_form', $values);
    if (!$worm_order_form) {
      drupal_set_message(t('Unable to add worm_order_form.'), 'warning');
      tripal_report_error('worm_order_form', TRIPAL_WARNING, 'Insert worm_order_form: Unable to create worm_order_form where values: %values',
        array('%values' => print_r($values, TRUE)));
      return;
    }

    // get the worm_order_form_id for linking Drupal node with Chado data
    $worm_order_form_id = $worm_order_form['worm_order_form_id'];

    // Only add to other Chado tables if the base record was inserted properly
    if ($worm_order_form_id > 0) {

      // If you implemented the properties form in chado_worm_order_form_form then you
      // need to handle inserting these properties into your Chado prop table.
      $details = array(
        // the name of the prop table
        'base_table'       => 'worm_order_form',
        // the name of the key in your base table
        'foreignkey_name'  => 'worm_order_form_id',
        // the value of the worm_order_form_id key
        'foreignkey_value' => $worm_order_form_id
      );
   }
  }
  else {
    // the node has an worm_order_form_id so get it for linking Drupal node with Chado
    // data
    $worm_order_form_id = $node->worm_order_form_id;
  }

  // Make sure the entry for this worm_order_form doesn't already exist in the
  // chado_worm_order_form table if it doesn't exist then we want to add it.
   $check_org_id = chado_get_id_from_nid('worm_order_form', $node->nid);
  if (!$check_org_id) {
    $record = new stdClass();
    $record->nid = $node->nid;
    $record->vid = $node->vid;
    $record->worm_order_form_id = $worm_order_form_id;
    drupal_write_record('chado_worm_order_form', $record);
  }
}

/**
 * Implementation of hook_update(). This function runs after the node has been
 * inserted into the Drupal schema and allows us to update the record in Chado.
 *
 * This function is not required if the hook_node_info() does not define any
 * custom node types.
 *
 * @ingroup worm_order_form
 */
function chado_worm_order_form_update($node) {
  // be sure to always trim text fields
  $worm_order_form_id = chado_get_id_from_nid('worm_order_form', $node->nid) ;
  // use the chado_update_record() function to update the record
  $match = array(
    'worm_order_form_id' => $worm_order_form_id,
  );
    $filepath = 'public://customs_file_upload';
    file_prepare_directory($filepath, FILE_CREATE_DIRECTORY);
    $safe_files = array('file_validate_extensions' => array('jpg jpeg gif png txt doc docx xls xlsx pdf ppt pps odt ods odp'));
    //$file = file_save_upload('customs_file_upload',null, $filepath);
    $file = file_save_upload('customs_file_upload',$safe_files, $filepath);
    $file_url = file_create_url($file->uri);
  $values = array(
    'worm_quantity' => $node->worm_quantity,
    'worm_biotype' => $node->worm_biotype ,
    'sstatus' => $node->sstatus,
    'date_shipped' => trim($node-> date_shipped),
    'airbill' => trim($node-> airbill),
    'shipper_name' => trim($node-> shipper_name),
    'contact_lab_head' => trim($node->contact_lab_head),
    'contact_position' => trim($node->contact_position),
    'delivery_address' => trim(preg_replace('/\s\s+/', ' ', $node->delivery_address['value'])),//  trim($node->delivery_address['value']),
    'delivery_phone' => trim($node->delivery_phone),
    'fedex_account' => trim($node->fedex_account),
    'carrier' => trim($node->carrier),
    'condition_acknowledgement' => $node->condition_acknowledgement,
    'receiver_name' => trim($node->receiver_name),
    'contact_name' => trim($node->contact_name),
    'contact_email' => trim($node->contact_email),
    'comments' => trim($node->comments['value']),
    'customs_file_upload' => trim($file_url),
    'status_notes' => trim($node->status_notes['value']),
  );
  $options = array('return_record' => TRUE);
  $update_status = chado_update_record('worm_order_form', $match, $values, $options);
  if (!$update_status) {
    drupal_set_message(t('Unable to update worm_order_form.'), 'warning');
    tripal_report_error('worm_order_form', TRIPAL_WARNING, 'Update worm_order_form: Unable to update worm_order_form where values: %values',
      array('%values' => print_r($values, TRUE)));
  }

  // If you implemented the properties form in chado_worm_order_form_form then you need
  // to handle updating these properties into your Chado prop table.
  $details = array(
    'base_table' => 'worm_order_form',         // the name of your Chado base table
    'foreignkey_name' => 'worm_order_form_id', // the name of the key in your base table
    'foreignkey_value' => $worm_order_form_id  // the value of the worm_order_form_id key
  );
  //chado_update_node_form_properties($node, $details);
/*
  // If you implemented the dbxrefs form in chado_worm_order_form_form then you need to
  // handle updating these database references into your Chado _dbxref table.
  $details = array(
    'linking_table' => 'worm_order_form_dbxref', // the name of your _dbxref table
    'foreignkey_name' => 'worm_order_form_id',   // the name of the key in your base table
    'foreignkey_value' => $worm_order_form_id    // the value of the worm_order_form_id key
  );
  chado_update_node_form_dbxrefs($node, $details);

  // If you implemented the relationships form in chado_worm_order_form_form then you
  // need to handle updating these relationships into your Chado _relationship
  // table.
  $details = array(
    // name of the _relationship table
    'relationship_table' => 'worm_order_form_relationship',
    // value of the worm_order_form_id key
    'foreignkey_value' => $worm_order_form_id
  );
  chado_update_node_form_relationships($node, $details);
*/
}
/**
 * Implementation of hook_delete(). This function runs after the node has been
 * deleted from the Drupal schema and allows us to delete the corresponding
 * record in Chado.
 *
 * This function is not required if the hook_node_info() does not define any
 * custom node types.
 *
 * @ingroup worm_order_form
 */
function chado_worm_order_form_delete($node) {

  // get the worm_order_form id from the node
  $worm_order_form_id  = chado_get_id_from_nid('worm_order_form', $node->nid);

  // if we don't have a worm_order_form id for this node then this isn't a node of type
  // chado_worm_order_form or the entry in the chado_worm_order_form table was lost.
  if (!$worm_order_form_id) {
    return;
  }

  // remove the entry in the chado_exapmle table linking the deleted
  // Drupal node with the data in Chado
  $sql_del = "DELETE FROM {chado_worm_order_form} WHERE nid = :nid AND vid = :vid";
  db_query($sql_del, array(':nid' => $node->nid, ':vid' => $node->vid));

  // Remove data from worm_order_form tables of Chado database. This will
  // cause a cascade delete and remove all data in referencing tables
  // for this worm_order_form
  chado_query("DELETE FROM {worm_order_form} WHERE worm_order_form_id = :worm_order_form_id", array(':worm_order_form_id' => $worm_order_form_id));

  // inform the user that the data was deleted
  drupal_set_message(t("The worm_order_form and all associated data were removed from Chado"));

}

/**
 * Implementation of hook_load(). This function is necessary to load into the
 * $node object the fields of the table form Chado. For worm_order_form for the worm_order_form
 * table, the chado_worm_order_form_load() function adds in a worm_order_form object which
 * contains all of the fields and sub objects for data in tables with foreign
 * key relationships.
 *
 * This function is not required if the hook_node_info() does not define any
 * custom node types.
 *
 * @ingroup worm_order_form
 */
function chado_worm_order_form_load($nodes) {
  // EXPLANATION: when displaying or node or accessing the node in a template
  // we need the data from Chado. This function finds the record in Chado that
  // this node belongs to and adds the record.

  // there may be multiple nodes that get passed in so we have to iterate
  // through them all
  foreach ($nodes as $nid => $node) {
    // find the worm_order_form and add in the details
    $worm_order_form_id = chado_get_id_from_nid('worm_order_form', $nid);

    // if the nid does not have a matching record then skip this node.
    // this can happen with orphaned nodes.
    if (!$worm_order_form_id) {
      continue;
    }

    // build the worm_order_form variable by using the chado_generate_var() function
    $values = array('worm_order_form_id' => $worm_order_form_id);
    $worm_order_form = chado_generate_var('worm_order_form', $values);

    // for fields in the table that are of type 'text' you may want to include
    // those by default, the chado_generate_var does not include text fields as
    // they may be very large and including a large text field can slow the page
    // load.
    // If you know a text field will never be large and it is important for the
    // other functions that will see the node to have access to a field you can
    // include it here using the chado_expand_var() function. In most
    // cases it is probably best to let the end-user decide if text fields
    // should be included by using this function in the templates.
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.worm_quantity');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.date_shipped');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.airbill');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.shipper_name');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.request_date');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.biotype');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.contact_lab_head');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.contact_position');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.delivery_address');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.delivery_phone');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.fedex_account');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.carrier_name');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.receiver_name');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.contact_name');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.contact_email');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.comments');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.customs_file_upload');
    $worm_order_form = chado_expand_var($worm_order_form, 'field', 'worm_order_form.status_notes');

    // add the new worm_order_form object to this node.
    $nodes[$nid]->worm_order_form = $worm_order_form;

    // If your module is using the Chado Node: Title & Path API to allow custom
    // titles for your node type. Every time you want the title of the node, you
    // need to use the following API function:
    $node->title = chado_get_node_title($node);

  }
}

/**
 * Implementation of hook_node_presave().
 *
 * Performs actions on a node object prior to it being saved
 *
 * @ingroup worm_order_form
 */
function worm_order_form_node_presave($node) {
  // EXPLANATION: This node is useful for making changes to the node prior to it
  // being saved to the database.
  // One useful case for this is to set the title of a node using values
  // supplied by the user.
  //
  // This function is not required. You probably won't need it if you don't
  // define a custom node type in the hook_node_info() function. But it is node
  // type agnostic, so you can use this function to change the contents of any
  // node regardless of it's type.

  // set the node title
  switch ($node->type) {
    // This step is for setting the title for the Drupal node. This title is
    // permanent and thus is created to be unique. Title changes provided by
    // tokens are generated on the fly dynamically, but the node title seen in
    // the content listing needs to be set here. Do not call the
    // chado_get_node_title() function here to set the title as the node object
    // isn't properly filled out and the function will fail.
    case 'chado_worm_order_form':
      // for a form submission the 'uniquename' field will be set,
      // for a sync, we must pull from the worm_order_form object
      $order_date = format_date($node->created, 'custom', "Y-m-d");
      if (property_exists($node, 'worm_quantity')) {
        // set the title
        //$library_term = tripal_get_cvterm(array('cvterm_id' => $node->worm_order_form_id));
        $biotype = 'Schmidtea mediterranea Asexuals';
        if ($node->worm_biotype == 1){
          $biotype = 'Schmidtea mediterranea Sexuals';
        }
        $node->title = "Worm Order: $node->worm_quantity $biotype by $node->contact_name $order_date";
     }
      else if (property_exists($node, 'worm_order_form')) {
        $node->title = "Worm Order: $node->worm_order_form->worm_quantity $biotype by $node->worm_order_form->contact_name $order_date";
      }
      break;
  }
}

/**
 * Implementation of hook node_insert().
 *
 * Performs actions after any node has been inserted.
 *
 * @ingroup worm_order_form
 */
function worm_order_form_node_insert($node) {
  // EXPLANATION: This function is used after any a node is inserted into the
  // database. It is different from the hook_insert() function above in that it
  // is called after any node is saved, regardless of it's type. This function
  // is useful for making changes to the database after a node is inserted.
  // An worm_order_form comes from the tripal_feature module where the URL alias of a
  // node cannot be set in the hook_insert() function. Therefore the
  // tripal_feature module uses this function to set the URL path of a newly
  // inserted worm_order_form node.
  //
  // This function is not required. You probably won't need it if you don't
  // define a custom node type in the hook_node_info() function. But it is node
  // type agnostic, so you can use this function to do any activity after insert
  // of any node.

  // the Ontology Term Submission code below will set the URL path after inserting. We do it here
  // because we do not know the worm_order_form_id in the pre-save and cannot do it in
  // the hook_insert()
  switch ($node->type) {
    case 'chado_worm_order_form':

      // find the worm_order_form and add in the details
      $worm_order_form_id = chado_get_id_from_nid('worm_order_form', $node->nid);
      // build the worm_order_form variable by using the chado_generate_var() function

      $biotype = 'Schmidtea mediterranea Asexuals';
      if ($node->worm_biotype == 1){
         $biotype = 'Schmidtea mediterranea Sexuals';
      }
      $carrier_name = 'FedEx';
      if ($node->carrier == 1){
         $carrier_name = 'UPS';
      }elseif($node->carrier == 2){
         $carrier_name = 'Other';
      }
      $values = array(
        'worm_order_form_id' => $worm_order_form_id,
        'request_date' => format_date($node->created, 'custom', "Y-m-d"),
        'biotype' => $biotype,
        'carrier_name' => $carrier_name,
   
      );
      $worm_order_form = chado_generate_var('worm_order_form', $values);
      $node->worm_order_form = $worm_order_form;

      // EXPLANATION: You can allow the site admin to customize the
      // title and URL of your node.  The 'Chado Node: Title & Path API'
      // contains two functions that can be called to generate the title and
      // URL based a schema provided by the site admin. These functions are
      // named chado_get_node_title() and chado_set_node_url().  These
      // functions use a string of tokens to build the URL and titles and the
      // site admin has the ability to set these tokens.  There are
      // form elements made available in the worm_order_form_admin() function
      // that allow the admin to set these tokens.  The default token string
      // is provided to Tripal using two hook functions, and are found below.
      // These are: chado_exmaple_chado_node_default_title() and
      // chado_worm_order_form_chdo_node_default_url().

      // Set the Title and URL for this node.
      $worm_order_form->title = chado_get_node_title($node);
      chado_set_node_url($node);
      break;
  }
}

/**
 * Implementation of hook node_update().
 *
 * Performs actions after any node has been updated.
 *
 */
function worm_order_form_node_update($node) {
  // EXPLANATION: This function is used after any a node is updated in the
  // database. It is different from the hook_update() function above in that it
  // is called after any node is updated, regardless of it's type.
  // An worm_order_form comes from the tripal_feature module where the URL alias of a
  // node cannot be set in the hook_update() function. Therefore the
  // tripal_feature module uses this function to reset the URL path of an
  // updated feature node.
  //
  // This function is not required. You probably won't need it if you don't
  // define a custom node type in the hook_node_info() function. But it is node
  // type agnostic, so you can use this function to do any activity after insert
  // of a node.

  // add items to other nodes, build index and search results
  switch ($node->type) {
    case 'chado_worm_order_form':

      // If your module is using the Chado Node: Title & Path API to allow
      // custom titles for your node type. Every time you want the title of the
      // node, you need to use the following API function:
      $worm_order_form->title = chado_get_node_title($node);

      // set the URL for this worm_order_form page
      // see the code in the tripal_feature/includes/tripal_feature.chado_node.inc
      // file in the function tripal_feature_node_insert for an worm_order_form of how
      // that module sets the URL. It uses a configuration file to allow the
      // user to dynamically build a URL schema and then uses that schema to
      // generate a URL string.
      break;
  }
}
/**
 * Implementation of hook_node_view().
 *
 * @ingroup worm_order_form
 */
function worm_order_form_node_view($node, $view_mode, $langcode) {
  // EXPLANATION: This function defines the content "blocks" that appear when
  // the node is displayed. It is node type agnostic so we can add content to
  // any node type. So, we use this function to add the content from all of our
  // theme templates onto our new node type. We will also use this function to
  // add content to other node types.

  switch ($node->type) {
    case 'chado_worm_order_form':
      // there are different ways a node can be viewed. Primarily Tripal
      // supports full page view and teaser view.
      if ($view_mode == 'full') {

        // If you want to use the default Tripal node template then you need to
        // tell Tripal to generate the Table of Contents. This is done by
        // setting the following to TRUE. If your content type follows the
        // chado_<base table> convention then this is the default. In this case
        // if you don't want to use the default template then you need to set
        // the following to FALSE.
        $node->content['#tripal_generic_node_template'] = TRUE;

        // There is always a base template. This is the template that is first
        // shown when the worm_order_form node type is first displayed.
        // If you are using the default Tripal node template, then you should
        // also set two additional items in each array:  tripal_toc_id and
        // tripal_toc_title. The tripal_tock_id should be a single unique
        // world that is used to reference the template. This ID is used for
        // constructing URLs for the content. The tripal_toc_title contains
        // the title that should appear in the table of contents for this
        // content. You should only set the '#weight' element for the base
        // template (or Overview) to ensure that it appears at the top of the
        // list. Otherwise items are sorted alphabetically.
        $node->content['worm_order_form_base'] = array(
          '#theme' => 'worm_order_form_base',
          '#node' => $node,
          '#tripal_toc_id'    => 'base',
          '#tripal_toc_title' => 'Overview',
          '#weight' => -100,
        );
        // we can add other templates as well for properties, publications,
        // dbxrefs, etc...
        
/*
        $node->content['worm_order_form_properties'] = array(
        );
        $node->content['worm_order_form_references'] = array(
          '#theme' => 'worm_order_form_references',
          '#node' => $node,
          '#tripal_toc_id'    => 'references',
          '#tripal_toc_title' => 'Cross References',
        );
        $node->content['worm_order_form_relationships'] = array(
          '#theme' => 'worm_order_form_relationships',
          '#node' => $node,
          '#tripal_toc_id'    => 'relationships',
          '#tripal_toc_title' => 'Relationships',
        );
*/
        // Note: if you create a template that you do not want a user to know
        // where it is (discourage editing of it), you can add the following
        // key:  '#tripal_template_show' => FALSE. If this key/value is set the
        // administrator message that Tripal provides indicating where the
        // template is housed will not be shown.
      }
      // set the content for the teaser view
      if ($view_mode == 'teaser') {
        // The teaser is also a required template
        $node->content['worm_order_form_teaser'] = array(
          '#theme' => 'worm_order_form_teaser',
          '#node' => $node,
        );
      }
      break;
    // you can add custom content to any node type by adding content to the node
    // in the same way as above.
  
    // ... etc
  }
}


/**
 * Implements [content_type]_chado_node_default_title_format().
 *
 * Defines a default title format for the Chado Node API to set the titles on
 * Chado worm_order_form nodes based on chado fields.
 */


function chado_worm_order_form_chado_node_default_title_format() {
  //return '[worm_order_form.uniquename] ([worm_order_form.type_id>cvterm.name]) [worm_order_form.organism_id>organism.genus] [worm_order_form.organism_id>organism.species]';
  return 'Worm Order [worm_order_form.worm_order_form_id]: [worm_order_form.worm_quantity] [worm_order_form.biotype] by [worm_order_form.contact_name] [worm_order_form.request_date]' ;
}

/**
 * Implements hook_chado_node_default_url_format().
 *
 * Designates a default URL format for worm_order_form nodes.
 */
function chado_worm_order_form_chado_node_default_url_format() {
//  return '/worm_order_form/[worm_order_form.type_id>cv.name]/[worm_order_form.term_name]';
  return '/worm_order_form/[worm_order_form.worm_order_form_id]';
}










