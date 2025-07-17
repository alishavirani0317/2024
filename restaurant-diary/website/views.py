# store standards routes where users can actually go to 

from flask import Blueprint, request, jsonify
from flask_login import login_required, current_user
from .models import RestaurantEntry
from . import db
import json

# this file is blueprint of our apps (has a ton of urls in it)

views = Blueprint('views', __name__)

# define route (view)
@views.route('/api/entries', methods=['GET'])
@login_required
def get_entries():
    entries = RestaurantEntry.query.filter_by(user_id=current_user.id).all()
    return jsonify([{
        'id': e.id,
        'restaurant_name': e.restaurant_name,
        'rating': e.rating,
        'notes': e.notes,
        'visit_date': e.visit_date.strftime('%Y-%m-%d')
    } for e in entries])

@views.route('/api/entries', methods=['POST'])
@login_required
def add_entry():
    data = request.get_json()
    entry = RestaurantEntry(
        restaurant_name=data.get('restaurant_name'),
        rating=int(data.get('rating')),
        notes=data.get('notes'),
        user_id=current_user.id
    )
    db.session.add(entry)
    db.session.commit()
    return jsonify({'message': 'Entry added'}), 201

@views.route('/api/entries/<int:entry_id>', methods=['DELETE'])
@login_required
def delete_entry(entry_id):
    entry = RestaurantEntry.query.get(entry_id)
    if entry and entry.user_id == current_user.id:
        db.session.delete(entry)
        db.session.commit()
        return jsonify({'message': 'Deleted'})
    return jsonify({'error': 'Not found'}), 404