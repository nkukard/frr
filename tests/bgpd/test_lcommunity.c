/*
 * Copyright (C) 2017 Nigel Kukard <nkukard@lbsd.net>
 * Copyright (C) 2007 Sun Microsystems, Inc.
 *
 * This file is part of Quagga.
 *
 * Quagga is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * Quagga is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Quagga; see the file COPYING.  If not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */
#include <zebra.h>

#include "vty.h"
#include "stream.h"
#include "privs.h"
#include "memory.h"
#include "queue.h"
#include "filter.h"

#include "bgpd/bgpd.h"
#include "bgpd/bgp_lcommunity.h"



/* need these to link in libbgp */
struct zebra_privs_t *bgpd_privs = NULL;
struct thread_master *master = NULL;

static int failed = 0;

/* specification for a test - what the results should be */
struct test_spec {
	const char *shouldbe; /* the string the path should parse to */
};


/* test segments to parse and validate, and use for other tests */
static struct test_segment {
	const char *name;
	const char *desc;
	const u_int8_t data[1024];
	int len;
	struct test_spec sp;
} test_segments[] = {
	/*
	{
		      "ipaddr",
		      "rt 1.2.3.4:257",
		      {ECOMMUNITY_ENCODE_IP, ECOMMUNITY_ROUTE_TARGET, 0x1, 0x2,
		       0x3, 0x4, 0x1, 0x1},
		      8,
		      {"rt 1.2.3.4:257"}},
		     {
		      "ipaddr-so",
		      "soo 1.2.3.4:257",
		      {ECOMMUNITY_ENCODE_IP, ECOMMUNITY_SITE_ORIGIN, 0x1, 0x2,
		       0x3, 0x4, 0x1, 0x1},
		      8,
		      {"soo 1.2.3.4:257"}},
		     {
		      "asn",
		      "rt 23456:987654321",
		      {ECOMMUNITY_ENCODE_AS, ECOMMUNITY_SITE_ORIGIN, 0x5b, 0xa0,
		       0x3a, 0xde, 0x68, 0xb1},
		      8,
		      {"soo 23456:987654321"}},
		     {
		      "asn4",
		      "rt 168450976:4321",
		      {ECOMMUNITY_ENCODE_AS4, ECOMMUNITY_SITE_ORIGIN, 0xa, 0xa,
		       0x5b, 0xa0, 0x10, 0xe1},
		      8,
		      {"soo 168450976:4321"}},
	*/
		     {NULL, NULL, {0}, 0, {NULL}}
};


/* validate the given aspath */
static int validate(struct lcommunity *lcom, const struct test_spec *sp)
{
	int fails = 0;
	struct lcommunity *ltmp;
	char *str1, *str2;

	printf("got:\n  %s\n", lcommunity_str(lcom));
	/*
	str1 = ecommunity_ecom2str(ecom, ECOMMUNITY_FORMAT_COMMUNITY_LIST, 0);
	etmp = ecommunity_str2com(str1, 0, 1);
	if (etmp)
		str2 = ecommunity_ecom2str(etmp,
					   ECOMMUNITY_FORMAT_COMMUNITY_LIST, 0);
	else
		str2 = NULL;

	if (strcmp(sp->shouldbe, str1)) {
		failed++;
		fails++;
		printf("shouldbe: %s\n%s\n", str1, sp->shouldbe);
	}
	if (!etmp || strcmp(str1, str2)) {
		failed++;
		fails++;
		printf("dogfood: in %s\n"
		       "    in->out %s\n",
		       str1, (etmp && str2) ? str2 : "NULL");
	}
	ecommunity_free(&etmp);
	XFREE(MTYPE_ECOMMUNITY_STR, str1);
	XFREE(MTYPE_ECOMMUNITY_STR, str2);
*/
	return fails;
}

static void test_new()
{
		// lcom to string test
		struct lcommunity *lcom;
		char *lcomstr;

		// lcom merge to string
		struct lcommunity *lcom2;
		struct lcommunity *lcom_merge;
		char *lcomstr_merge;

		// lcom uniq to string test
		struct lcommunity *lcom_uniq;
		char *lcomstr_uniq;

		// lcom delete test
		struct lcommunity *lcom_del;
		struct lcommunity *lcom_del_item;
		char *lcomstr_del;


		lcom = lcommunity_str2com("1:1:1 1:10:1 1:1:2");
		lcomstr = lcommunity_lcom2str(lcom,LCOMMUNITY_FORMAT_DISPLAY);
		printf("lcom2str[%i]: >%s<\n",lcom->size,lcomstr);

		lcom2 = lcommunity_str2com("1:1:1 1:10:1 1:1:2 1:1:5 1:9:1");
		lcom_merge = lcommunity_merge(lcom2,lcom);
		lcomstr_merge = lcommunity_lcom2str(lcom_merge,LCOMMUNITY_FORMAT_DISPLAY);
		printf("merge[%i]: >%s<\n",lcom_merge->size,lcomstr_merge);

		lcom_uniq = lcommunity_uniq_sort(lcom_merge);
		lcomstr_uniq = lcommunity_lcom2str(lcom_uniq,LCOMMUNITY_FORMAT_DISPLAY);
		printf("uniq_sort[%i]: >%s<\n",lcom_uniq->size,lcomstr_uniq);


		lcom_del = lcommunity_dup(lcom_uniq);
		lcom_del_item = lcommunity_str2com("1:1:2");
		lcommunity_del_val(lcom_del,lcom_del_item->val);
		lcom_del_item = lcommunity_str2com("1:10:1");
		lcommunity_del_val(lcom_del,lcom_del_item->val);
		lcom_del_item = lcommunity_str2com("1:1:5");
		lcommunity_del_val(lcom_del,lcom_del_item->val);
		lcom_del_item = lcommunity_str2com("1:9:1");
		lcommunity_del_val(lcom_del,lcom_del_item->val);

		lcomstr_del = lcommunity_lcom2str(lcom_del,LCOMMUNITY_FORMAT_DISPLAY);
		printf("del_val[%i]: >%s<\n",lcom_del->size,lcomstr_del);
}



/* basic parsing test */
static void parse_test(struct test_segment *t)
{
	struct lcommunity *lcom;

	printf("%s: %s\n", t->name, t->desc);
/*
	ecom = ecommunity_parse((u_int8_t *)t->data, t->len);

	printf("ecom: %s\nvalidating...:\n", ecommunity_str(ecom));

	if (!validate(ecom, &t->sp))
		printf("OK\n");
	else
		printf("failed\n");

	printf("\n");
	ecommunity_unintern(&ecom);
	*/
}


int main(void)
{
	int i = 0;

	// Initialize hash
	lcommunity_init();

	test_new();
/*
	while (test_segments[i].name)
		parse_test(&test_segments[i++]);
*/

	printf("failures: %d\n", failed);
	// printf ("aspath count: %ld\n", aspath_count());
	return failed;
	// return (failed + aspath_count());
}
