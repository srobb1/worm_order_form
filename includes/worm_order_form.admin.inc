<?php
/**
 * @file
 * This file contains the functions used for administration of the module
 *
 */

function worm_order_form_admin_examples_listing() {
  $output = '';

  // set the breadcrumb
  $breadcrumb = array();
  $breadcrumb[] = l('Home', '<front>');
  $breadcrumb[] = l('Administration', 'admin');
  $breadcrumb[] = l('Tripal', 'admin/tripal');
  $breadcrumb[] = l('Chado', 'admin/tripal/chado');
  $breadcrumb[] = l('Examples', 'admin/tripal/chado/worm_order_form');
  drupal_set_breadcrumb($breadcrumb);


  // EXPLANATION:  Typically for all Tripal modules the home administrative page
  // for the module contains a search form to help the adminstrator locate
  // records. The following example code adds a default View to the page
  /*
  // Add the view
  $view = views_embed_view('worm_order_form_admin_examples','default');
  if (isset($view)) {
    $output .= $view;
  }
  else {
    $output .= '<p>The Tripal Example Module uses primarily views to provide an '
      . 'administrative interface. Currently one or more views needed for this '
      . 'administrative interface are disabled. <strong>Click each of the following links to '
      . 'enable the pertinent views</strong>:</p>';
    $output .= '<ul>';
      $output .= '<li>'.l('Example Admin', 'admin/tripal/chado/worm_order_form/views/examples/enable').'</li>';
    $output .= '</ul>';
  }
  */
  $output = 'Typically a search view goes here';

  return $output;
}

/**
 * Administrative settings form
 *
 * @ingroup worm_order_form
 */
function worm_order_form_admin() {
  $form = array();

  $form['nothing'] = array(
    '#markup' => t('There are currently no settings to configure.')
  );

  // If your module is using the Chado Node: Title & Path API to allow custom
  // titles for your node type then you need to add the configuration form for
  // this functionality. To do so, we first have to preapre a $details array
  // the describe our node type.  Then we call the function to create the form
  // elements.
  $details = array(
      // the name of the MODULE implementing the content type
    'module' => 'worm_order_form',
    'content_type' => 'chado_worm_order_form',
      // An array of options to use under "Page Titles"
      // the key should be the token and the value should be the human-readable
      // option
    'options' => array(
      '[worm_order_form.term_name]' => 'Term Name',
      '[worm_order_form.contact_name]' => 'Germplasm Unique Name Only',
        // there should always be one options matching the unique constraint.
        // If you have a more human-readable constraint, then that is
        // preferable.
        // See the tripal feature module for a good example of this.
      '[worm_order_form.onto_term_submit_id]' => 'Unique Constraint: The Chado ID for Examples',
      '[worm_order_form.type_id>cv.name]' => 'Unique Constraint: The Chado ID for Examples',
    ),
    // the token indicating the unique constraint in the options array
    'unique_option' => '[worm_order_form.onto_term_submit_id]'
  );
  // This call adds the configuration form to your current form
  // This sub-form handles it's own validation & submit
  chado_add_admin_form_set_title($form, $form_state, $details);


  // FEATURE NODE URL
  // Using the Chado Node: Title & Path API
  $details = array(
    'module' => 'worm_order_form',
    'content_type' => 'chado_worm_order_form',
      // An array of options to use under "Page URL"
      // the key should be the token and the value should be the human-readable option
    'options' => array(
      '/worm_order_form/[onto_term_submit.onto_term_submit_id]' => 'Feature ID',
        // there should always be one options matching the unique constraint.
      '/worm_order_form/[onto_term_submit.type_id>cv.name]/[onto_term_submit.term_name]' => 'Unique Contraint: Includes the name, uniquename, type and scientific name'
    )
  );
  // This call adds the configuration form to your current form
  // This sub-form handles it's own validation & submit
  chado_add_admin_form_set_url($form, $form_state, $details);


  return system_settings_form($form);
}



/**
 *
 * @ingroup worm_order_form
 */
function worm_order_form_admin_validate($form, &$form_state) {

}