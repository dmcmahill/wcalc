/* $Id$ */

/* ********* Automatically Generated.  Do not edit! ******** */
/* *********         Created with html2c            ******** */



fprintf(fp,"\n");
fprintf(fp,"%%\n");
fprintf(fp,"%% All done with data output.\n");
fprintf(fp,"%%\n");
fprintf(fp,"\n");
fprintf(fp,"%%\n");
fprintf(fp,"%% add the time stamp\n");
fprintf(fp,"%%\n");
fprintf(fp,"\n");
fprintf(fp,"%g inch 2 div\n",paperwidth);
fprintf(fp,"%g inch \n",bottommargin);
fprintf(fp,"moveto\n");
fprintf(fp,"(%s) centerbotshow\n",ctime(&now));
fprintf(fp,"\n");
fprintf(fp,"showpage\n");
fprintf(fp,"\n");
fprintf(fp,"%%%%Trailer\n");
fprintf(fp,"%%MatchingCreationDate: \n");
fprintf(fp,"%%%%DocumentFonts: Symbol\n");
fprintf(fp,"%%%%+ Helvetica Times-Roman\n");
fprintf(fp,"%%%%EOF\n");

/* *********        End of Generated Code           ******** */

