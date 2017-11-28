// =-=-=-=-=-=-=-
#include "msParam.hpp"
#include "reGlobalsExtern.hpp"
#include "irods_ms_plugin.hpp"
#include "netcdfMS.hpp"

// =-=-=-=-=-=-=-
// STL Includes
#include <iostream>

extern "C" {

    // =-=-=-=-=-=-=-
    // 1. Set the version of the plugin.  1.0 is currently the standard

    // =-=-=-=-=-=-=-
    // 2. Write a standard issue microservice


    /**
    * \fn msiFloatToString (msParam_t *floatParam, msParam_t *stringParam, ruleExecInfo_t *rei)
    * \brief Convert a floating point number to a string because writeLine does not yet support floating point output.
    * \module core
    *
    * \since 3.2
    *
    * \author  Mike Wan
    * \date    2012
    *
    * \usage See clients/icommands/test/rules3.0/netcdfTest1.r, netcdfTest2.r and netcdfTest3.r.
    * \param[in] floatParam - A FLOAT_MS_T containing the floating point number
    * \param[out] stringParam - A STR_MS_T containing the output string
    *
    * \DolVarDependence none
    * \DolVarModified none
    * \iCatAttrDependence none
    * \iCatAttrModified none
    * \sideeffect none
    *
    * \return integer
    * \retval 0 upon success
    * \pre N/A
    * \post N/A
    * \sa N/A
    *
    **/
    int
    msiFloatToString (msParam_t *floatParam, msParam_t *stringParam,
    ruleExecInfo_t *rei)
    {
        char floatStr[NAME_LEN];
        float *myfloat;

        RE_TEST_MACRO ("    Calling msiFloatToString")

        if (floatParam == NULL || stringParam == NULL) return USER__NULL_INPUT_ERR;

        if (strcmp (floatParam->type, FLOAT_MS_T) != 0) {
            rodsLog (LOG_ERROR,
              "msiFloatToString: floatParam type %s error", floatParam->type);
            return USER_PARAM_TYPE_ERR;
        }
        myfloat = (float *) floatParam->inOutStruct;
        snprintf (floatStr, NAME_LEN, "%f", *myfloat);
        fillStrInMsParam (stringParam, floatStr);

        return 0;
    }

    // =-=-=-=-=-=-=-
    // 3.  Create the plugin factory function which will return a microservice
    //     table entry
    irods::ms_table_entry*  plugin_factory( ) {
        // =-=-=-=-=-=-=-
        // 4.  allocate a microservice plugin which takes the number of function
        //     params as a parameter to the constructor
        irods::ms_table_entry* msvc = new irods::ms_table_entry( 2 );

        // =-=-=-=-=-=-=-
        // 5. add the microservice function as an operation to the plugin
        //    the first param is the name / key of the operation, the second
        //    is the name of the function which will be the microservice
        msvc->add_operation( "msiFloatToString", "msiFloatToString" );

        // =-=-=-=-=-=-=-
        // 6. return the newly created microservice plugin
        return msvc;
    }

}; // extern "C"



