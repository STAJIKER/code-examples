from __future__ import print_function

import sys
from operator import add

from pyspark import SparkContext


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: twitter <input1> <input2> <output>", file=sys.stderr)
        exit(-1)

    input_path1 = sys.argv[1]
    input_path2 = sys.argv[2]
    output_path = sys.argv[3]

    sc = SparkContext(appName="Twitter")

    lines = sc.textFile(input_path1)

    lines_data = lines.map(lambda line: line.split('\t'))
    number_followers = lines_data.map(lambda (user_id, follower_id): (user_id, 1)).reduceByKey(add).cache()
    number_users = number_followers.count()
    sum_followers = number_followers.map(lambda (user_id, count): (1, count)).reduceByKey(add).collectAsMap()[1]

    sc.parallelize([float(sum_followers) / number_users]).saveAsTextFile(output_path + "/avg_follower_count")

    left = 0
    right = 10
    intervals = []
    for i in xrange(0, 10):
        current_interval = number_followers.filter(lambda (user_id, count): count > left and count <= right)
        current_number = current_interval.count()
        if left == 0:
            left += 1
            intervals.append("[%d, %d]\t%d" % (left, right, current_number))
        else:
            intervals.append("(%d, %d]\t%d" % (left, right, current_number))
        left *= 10
        right *= 10

    sc.parallelize(intervals).saveAsTextFile(output_path + "/intervals")

    top_ids = sc.parallelize(number_followers.filter(lambda (user_id, count): count > 1000).sortBy(lambda pair: pair[1], ascending=False).take(50))
    names = sc.textFile(input_path2)
    names_data = names.map(lambda line: line.split(' '))
    top_names = top_ids.join(names_data).map(lambda (user_id, (count, name)): (count, "%s\t%s\t%d" % (user_id, name, count)))
    top_results = top_names.sortBy(lambda pair: pair[0], ascending=False)

    top_results.map(lambda (count, line): line).coalesce(1).saveAsTextFile(output_path + "/top50")

    sc.stop()